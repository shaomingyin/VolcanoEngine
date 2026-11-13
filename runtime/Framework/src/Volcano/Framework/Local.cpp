#include <algorithm>
#include <filesystem>
#include <functional>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <Volcano/Gui/Document.h>
#include <Volcano/World/Canvas.h>
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Model.h>
#include <Volcano/World/OrthographicCamera.h>
#include <Volcano/World/Overlay.h>
#include <Volcano/World/PerspectiveCamera.h>
#include <Volcano/Framework/Drawing.h>
#include <Volcano/Framework/Local.h>
#include <Volcano/Framework/Resources.h>

VOLCANO_FRAMEWORK_BEGIN

Size Local::cameraOutputSize(const World::Scene& scene, entt::entity entity) {
    if (!scene.valid(entity)) {
        return {};
    }
    if (const auto* perspective = scene.try_get<World::PerspectiveCamera>(entity)) {
        return perspective->outputSize();
    }
    if (const auto* orthographic = scene.try_get<World::OrthographicCamera>(entity)) {
        return orthographic->outputSize();
    }
    return {};
}

Size Local::outputSize(const World::Scene& scene, entt::entity entity) {
    if (!scene.valid(entity)) {
        return {};
    }
    if (const auto* doc = scene.try_get<Gui::Document>(entity);
        doc != nullptr && !scene.all_of<World::Overlay>(entity)) {
        return doc->size();
    }
    if (const auto* drawing = scene.try_get<Framework::Drawing>(entity)) {
        return drawing->size();
    }
    return cameraOutputSize(scene, entity);
}

bool Local::hasOutput(const World::Scene& scene, entt::entity entity) {
    if (!scene.valid(entity)) {
        return false;
    }
    if (scene.try_get<Gui::Document>(entity) != nullptr && !scene.all_of<World::Overlay>(entity)) {
        return true;
    }
    if (scene.try_get<Framework::Drawing>(entity) != nullptr) {
        return true;
    }
    return !cameraOutputSize(scene, entity).empty();
}

entt::entity Local::findOutputEntity(const World::Scene& scene, std::string_view name) {
    for (const auto entity : scene.view<Framework::Drawing>()) {
        if (scene.get<Framework::Drawing>(entity).name() == name) {
            return entity;
        }
    }
    for (const auto entity : scene.view<World::Inherent>()) {
        if (scene.get<World::Inherent>(entity).name() == name && hasOutput(scene, entity)) {
            return entity;
        }
    }
    return entt::null;
}

entt::entity Local::findGuiEntity(const World::Scene& scene, std::string_view name) {
    for (const auto entity : scene.view<World::Inherent, Gui::Document>()) {
        if (scene.get<World::Inherent>(entity).name() == name) {
            return entity;
        }
    }
    return entt::null;
}

std::string Local::outputLabel(const World::Scene& scene, entt::entity entity) {
    if (scene.valid(entity)) {
        if (const auto* drawing = scene.try_get<Framework::Drawing>(entity)) {
            return drawing->name();
        }
        if (const auto* inherent = scene.try_get<World::Inherent>(entity);
            inherent != nullptr && !inherent->name().empty()) {
            return inherent->name();
        }
    }
    return "entity:" + std::to_string(static_cast<std::uint32_t>(entt::to_integral(entity)));
}

Local::Local(int width, int height, const char* title)
    : window_(width, height, title) {
    renderer_.setFontResolver([this](std::string_view name) -> const Graphics::Font* {
        if (name.empty()) {
            return nullptr;
        }
        const auto normal = std::filesystem::path(name).lexically_normal();
        const auto it = fonts_.find(normal);
        return it != fonts_.end() ? it->second.get() : nullptr;
    });
    renderer_.setTextureResolver([this](const World::Canvas::Command& cmd) -> const Graphics::Texture* {
        if (!cmd.texture.empty()) {
            return &texture(cmd.texture);
        }
        return nullptr;
    });
}

Local::~Local() {
    window_.makeCurrent();
    // Cancel and settle GL continuations while Window and Renderer are still alive.
    cancelLoad();
    try {
        waitForLoad();
    } catch (const LoadCanceled&) {
    } catch (const std::exception& error) {
        spdlog::error("Resource loading failed during shutdown: {}", error.what());
    }
    unload();
    drawing_providers_.clear();
}

bool Local::shouldClose() const noexcept {
    return Base::shouldClose() || glfwWindowShouldClose(window_.handle());
}

const Graphics::Texture& Local::texture(entt::entity entity) const {
    window_.makeCurrent();
    if (const auto found = outputs_.find(entity); found != outputs_.end()) {
        return found->second->target.texture();
    }
    throw std::out_of_range("Texture not found for entity: " +
        std::to_string(static_cast<std::uint32_t>(entt::to_integral(entity))));
}

const Graphics::Texture& Local::texture(std::string_view id) const {
    window_.makeCurrent();
    if (output_scene_ != nullptr) {
        const auto entity = findOutputEntity(scene(), id);
        if (entity != entt::null) {
            if (const auto found = outputs_.find(entity); found != outputs_.end()) {
                return found->second->target.texture();
            }
        }
    }
    const auto normal = std::filesystem::path(id).lexically_normal();
    if (const auto found = textures_.find(normal); found != textures_.end()) {
        return *found->second;
    }
    throw std::out_of_range("Texture not found: " + std::string(id));
}

void Local::setDrawingProvider(std::string id, std::function<void(World::Canvas&)> provider) {
    window_.makeCurrent();
    if (id.empty() || !provider) {
        throw std::invalid_argument("Drawing provider requires an ID and callback");
    }
    drawing_providers_.insert_or_assign(std::move(id), std::move(provider));
}

void Local::render(entt::entity camera) {
    render(window_, camera);
}

void Local::onBeforeTasks() {
    window_.makeCurrent();
}

exec::task<void> Local::onLoadScene(const World::Scene& candidate) {
    checkLoadCancelled();
    pending_output_scene_ = &candidate;
    co_await Base::onLoadScene(candidate);
    std::vector<entt::entity> ids;

    for (const auto entity : candidate.view<Gui::Document>()) {
        checkLoadCancelled();
        const auto& doc = candidate.get<Gui::Document>(entity);
        if (!candidate.all_of<World::Overlay>(entity)) {
            setLoadingContent(outputLabel(candidate, entity));
            ids.push_back(entity);
            loadOutput(entity, doc.size());
        }

        std::vector<std::filesystem::path> fonts;
        std::vector<std::string> images;
        doc.root().collectResources(fonts, images);
        for (const auto& path : fonts) {
            checkLoadCancelled();
            setLoadingContent(path.generic_string());
            pending_fonts_.insert_or_assign(path.lexically_normal(),
                Resources::loadFont(path.generic_string()));
        }
    }

    for (const auto entity : candidate.view<Framework::Drawing>()) {
        checkLoadCancelled();
        const auto& drawing = candidate.get<Framework::Drawing>(entity);
        setLoadingContent(drawing.name());
        ids.push_back(entity);
        loadOutput(entity, drawing.size());

        for (const auto& path : drawing.fonts()) {
            checkLoadCancelled();
            setLoadingContent(path.generic_string());
            pending_fonts_.insert_or_assign(path.lexically_normal(),
                Resources::loadFont(path.generic_string()));
        }
        try {
            drawing_providers_.at(drawing.provider());
        } catch (const std::out_of_range&) {
            throw std::invalid_argument("Missing drawing provider: " + drawing.provider());
        }
    }

    for (const auto entity : candidate.view<World::PerspectiveCamera>()) {
        checkLoadCancelled();
        const auto size = candidate.get<World::PerspectiveCamera>(entity).outputSize();
        if (!size.empty()) {
            setLoadingContent(outputLabel(candidate, entity));
            ids.push_back(entity);
            loadOutput(entity, size);
        }
    }

    for (const auto entity : candidate.view<World::OrthographicCamera>()) {
        checkLoadCancelled();
        const auto size = candidate.get<World::OrthographicCamera>(entity).outputSize();
        if (!size.empty()) {
            setLoadingContent(outputLabel(candidate, entity));
            ids.push_back(entity);
            loadOutput(entity, size);
        }
    }

    checkLoadCancelled();
    textureOrder(candidate, ids);
}

exec::task<void> Local::onLoadEntity(const World::Scene& candidate, entt::entity entity) {
    co_await Base::onLoadEntity(candidate, entity);
    checkLoadCancelled();
    const auto* model = candidate.try_get<World::Model>(entity);
    if (model == nullptr) {
        co_return;
    }
    const auto& parts = this->model(model->source()).parts();
    if (model->materials().size() > parts.size()) {
        throw std::invalid_argument("Model material override part index is out of range");
    }
    for (const auto& material : model->materials()) {
        for (const auto& [slot, sourceEntity] : material.textures()) {
            if (!hasOutput(candidate, sourceEntity)) {
                throw std::invalid_argument(
                    "Model material texture override must reference a valid dynamic output entity");
            }
        }
    }
    std::vector<Graphics::Model::Part> resolvedParts;
    resolvedParts.reserve(parts.size());
    for (const auto& part : parts) {
        checkLoadCancelled();
        setLoadingContent(part.mesh());
        const auto meshPath = std::filesystem::path(part.mesh()).lexically_normal();
        auto mesh = Resources::loadMesh(part.mesh());
        Graphics::Model::Part resolvedPart;
        resolvedPart.mesh = mesh.get();
        pending_meshes_.insert_or_assign(meshPath, std::move(mesh));

        checkLoadCancelled();
        setLoadingContent(part.material());
        auto resource = Resources::loadMaterial(part.material());
        const auto& material = *resource;
        const auto materialPath = std::filesystem::path(part.material()).lexically_normal();
        pending_materials_.insert_or_assign(materialPath, std::move(resource));

        if (material.has_albedo()) {
            resolvedPart.material.color = Colorf(
                material.albedo().x(),
                material.albedo().y(),
                material.albedo().z(),
                material.albedo().w());
        } else {
            resolvedPart.material.color = Colorf::White;
        }
        resolvedPart.material.lit = material.has_lit() ? material.lit() : true;
        resolvedPart.transparent = material.alpha_mode() == Asset::ALPHA_MODE_BLEND;

        if (!material.albedo_texture().empty()) {
            checkLoadCancelled();
            setLoadingContent(material.albedo_texture());
            auto albedo = Resources::loadTexture(material.albedo_texture());
            resolvedPart.material.texture = albedo.get();
            pending_textures_.insert_or_assign(
                std::filesystem::path(material.albedo_texture()).lexically_normal(),
                std::move(albedo));
        }
        for (const auto& path : {material.normal_texture(), material.metallic_roughness_texture()}) {
            checkLoadCancelled();
            if (!path.empty()) {
                setLoadingContent(path);
                pending_textures_.insert_or_assign(std::filesystem::path(path).lexically_normal(),
                    Resources::loadTexture(path));
            }
        }

        if (part.transform_size() == 16) {
            for (int r = 0; r < 4; ++r) {
                for (int c = 0; c < 4; ++c) {
                    resolvedPart.transform.matrix()(r, c) = part.transform(r * 4 + c);
                }
            }
        }
        resolvedParts.push_back(std::move(resolvedPart));
    }
    pending_render_models_.insert_or_assign(
        entity, Graphics::Model(model->source(), std::move(resolvedParts)));
}

void Local::onCommitResources() noexcept {
    view_.begin();
    auto* target = currentScene();
    VOLCANO_ASSERT(target != nullptr);
    if (output_scene_ != pending_output_scene_) {
        outputs_.swap(pending_outputs_);
        pending_outputs_.clear();
    } else {
        outputs_.merge(pending_outputs_);
        pending_outputs_.clear();
        if (pending_output_scene_ != nullptr) {
            std::erase_if(outputs_, [&](const auto& entry) {
                return !hasOutput(*pending_output_scene_, entry.first);
            });
        }
    }
    for (auto& [entity, output] : outputs_) {
        output->dirty = true;
    }
    output_scene_ = pending_output_scene_;
    pending_output_scene_ = nullptr;
    fonts_.swap(pending_fonts_);
    pending_fonts_.clear();
    materials_.swap(pending_materials_);
    pending_materials_.clear();
    meshes_.swap(pending_meshes_);
    pending_meshes_.clear();
    textures_.swap(pending_textures_);
    pending_textures_.clear();

    target->clear<Graphics::Model>();
    for (auto& [entity, model] : pending_render_models_) {
        if (target->valid(entity)) {
            target->emplace<Graphics::Model>(entity, std::move(model));
        }
    }
    pending_render_models_.clear();

    target->clear<Graphics::View::Output>();
    for (const auto& [entity, output] : outputs_) {
        if (target->valid(entity)) {
            target->emplace<Graphics::View::Output>(entity, &output->target.texture());
        }
    }
}

void Local::onDiscardResources() noexcept {
    pending_fonts_.clear();
    pending_materials_.clear();
    pending_meshes_.clear();
    pending_textures_.clear();
    pending_render_models_.clear();
    pending_outputs_.clear();
    pending_output_scene_ = nullptr;
}

void Local::onUnloadResources() noexcept {
    window_.makeCurrent();
    view_.begin();
    if (auto* target = currentScene(); target != nullptr) {
        target->clear<Graphics::Model>();
        target->clear<Graphics::View::Output>();
    }
    pending_render_models_.clear();
    outputs_.clear();
    pending_outputs_.clear();
    output_scene_ = nullptr;
    pending_output_scene_ = nullptr;
    pending_fonts_.clear();
    fonts_.clear();
    pending_materials_.clear();
    materials_.clear();
    pending_meshes_.clear();
    meshes_.clear();
    pending_textures_.clear();
    textures_.clear();
}

void Local::onBeforeUpdate(Clock::duration elapsed) {
    Base::onBeforeUpdate(elapsed);
    onInputUpdate(elapsed);
}

void Local::onAfterUpdate(Clock::duration elapsed) {
    onAudioUpdate(elapsed);
    Base::onAfterUpdate(elapsed);
}

void Local::onInputUpdate(Clock::duration) {
}

void Local::onAudioUpdate(Clock::duration) {
}

void Local::updateTextures(const std::vector<entt::entity>& roots) {
    std::set<entt::entity> updated;
    for (const auto entity : textureOrder(scene(), roots)) {
        if (!outputs_.contains(entity)) {
            continue;
        }
        auto& output = *outputs_.at(entity);
        bool dependencyChanged = false;
        for (const auto dependency : dependencies(scene(), entity)) {
            dependencyChanged = dependencyChanged || updated.contains(dependency);
        }
        if (auto* doc = scene().try_get<Gui::Document>(entity);
            doc != nullptr && !scene().all_of<World::Overlay>(entity)) {
            const auto size = doc->size();
            if (output.target.size() != size) {
                output.target.resize(size.width, size.height);
                output.dirty = true;
            }
            auto revision = doc->revision();
            std::vector<std::filesystem::path> fontPaths;
            std::vector<std::string> imagePaths;
            doc->root().collectResources(fontPaths, imagePaths);
            for (const auto& path : fontPaths) {
                revision += fonts_.at(path.lexically_normal())->revision();
            }
            if (!output.dirty && !dependencyChanged && output.revision == revision) {
                continue;
            }
            output.dirty = true;
            doc->layout({static_cast<float>(size.width), static_cast<float>(size.height)});
            doc->draw();
            output.revision = revision;
            renderer_.renderVG(*doc, output.target);
        } else if (const auto* drawing = scene().try_get<Framework::Drawing>(entity)) {
            const auto size = drawing->size();
            if (output.target.size() != size) {
                output.target.resize(size.width, size.height);
                output.dirty = true;
            }
            output.canvas.begin();
            drawing_providers_.at(drawing->provider())(output.canvas);
            output.canvas.end();
            validateSamples(output.canvas, drawing->dependencies());
            renderer_.renderVG(output.canvas, output.target);
        } else if (const auto cameraSize = cameraOutputSize(scene(), entity); !cameraSize.empty()) {
            if (output.target.size() != cameraSize) {
                output.target.resize(cameraSize.width, cameraSize.height);
                output.dirty = true;
            }
            Graphics::View rttView;
            rttView.extract(scene(), entity, entity);
            renderer_.render(rttView, output.target);
        }
        output.dirty = false;
        updated.insert(entity);
    }
}

void Local::render(Graphics::Target& target, entt::entity camera) {
    window_.makeCurrent();
    std::vector<entt::entity> roots;
    for (const auto entity : scene().view<World::Model>()) {
        if (Graphics::View::visible(scene(), entity)) {
            for (const auto& material : scene().get<World::Model>(entity).materials()) {
                for (const auto& [slot, sourceEntity] : material.textures()) {
                    if (scene().valid(sourceEntity)) {
                        roots.push_back(sourceEntity);
                    }
                }
            }
        }
    }
    for (const auto entity : scene().view<World::Overlay>()) {
        if (Graphics::View::visible(scene(), entity)) {
            for (const auto dependency : dependencies(scene(), entity)) {
                roots.push_back(dependency);
            }
        }
    }
    for (const auto& [entity, output] : outputs_) {
        if (scene().valid(entity) && Graphics::View::visible(scene(), entity)) {
            roots.push_back(entity);
        }
    }
    updateTextures(roots);

    std::vector<std::reference_wrapper<const World::Canvas>> guiOverlays;
    for (const auto entity : scene().view<World::Overlay, Gui::Document>()) {
        if (!Graphics::View::visible(scene(), entity)) {
            continue;
        }
        auto& doc = scene().get<Gui::Document>(entity);
        const Size docSize = doc.size().empty() ? target.size() : doc.size();
        if (docSize.width > 0 && docSize.height > 0) {
            doc.layout({static_cast<float>(docSize.width), static_cast<float>(docSize.height)});
            doc.draw();
            guiOverlays.push_back(doc);
        }
    }
    view_.extract(scene(), camera, entt::null, guiOverlays);
    renderer_.render(view_, target);
}

bool Local::pointer(entt::entity entity, const Gui::PointerEvent& event) {
    if (!scene().valid(entity)) {
        throw std::invalid_argument("Invalid GUI entity");
    }
    auto* doc = scene().try_get<Gui::Document>(entity);
    if (doc == nullptr) {
        throw std::invalid_argument("Entity is not a GUI document: " + outputLabel(scene(), entity));
    }
    const auto size = doc->size().empty() ? window_.size() : doc->size();
    doc->layout({static_cast<float>(size.width), static_cast<float>(size.height)});
    return doc->event(event);
}

bool Local::pointer(std::string_view id, const Gui::PointerEvent& event) {
    const auto entity = findGuiEntity(scene(), id);
    if (entity == entt::null) {
        throw std::invalid_argument("Texture is not a GUI texture: " + std::string(id));
    }
    return pointer(entity, event);
}

bool Local::key(entt::entity entity, const Gui::KeyEvent& event) {
    if (!scene().valid(entity)) {
        throw std::invalid_argument("Invalid GUI entity");
    }
    auto* doc = scene().try_get<Gui::Document>(entity);
    if (doc == nullptr) {
        throw std::invalid_argument("Entity is not a GUI document: " + outputLabel(scene(), entity));
    }
    return doc->event(event);
}

bool Local::key(std::string_view id, const Gui::KeyEvent& event) {
    const auto entity = findGuiEntity(scene(), id);
    if (entity == entt::null) {
        throw std::invalid_argument("Texture is not a GUI texture: " + std::string(id));
    }
    return key(entity, event);
}

Local::TextureOutput::TextureOutput(Size size) : target(size.width, size.height) {}

void Local::loadOutput(entt::entity entity, Size size) {
    VOLCANO_ASSERT(pending_output_scene_ != nullptr);
    if ((output_scene_ != pending_output_scene_ || !outputs_.contains(entity)) &&
        !pending_outputs_.contains(entity)) {
        pending_outputs_.emplace(entity, std::make_unique<TextureOutput>(size));
    }
}

void Local::validateSamples(const World::Canvas& canvas, const std::vector<std::string>& dependencies) const {
    for (const auto& cmd : canvas.commands()) {
        if (cmd.type == World::Canvas::Command::Type::Image && !cmd.texture.empty()) {
            const auto sampled = findOutputEntity(scene(), cmd.texture);
            if (sampled != entt::null && outputs_.contains(sampled) &&
                std::find(dependencies.begin(), dependencies.end(), cmd.texture) == dependencies.end()) {
                throw std::invalid_argument("Drawing texture sampled an undeclared dependency: " + cmd.texture);
            }
        }
    }
}

std::vector<entt::entity> Local::dependencies(const World::Scene& current, entt::entity entity) {
    std::vector<entt::entity> result;
    if (const auto* drawing = current.try_get<Framework::Drawing>(entity)) {
        for (const auto& name : drawing->dependencies()) {
            const auto dependency = findOutputEntity(current, name);
            if (dependency != entt::null) {
                result.push_back(dependency);
            }
        }
        return result;
    }
    if (const auto* doc = current.try_get<Gui::Document>(entity)) {
        std::vector<std::filesystem::path> fonts;
        std::vector<std::string> images;
        doc->root().collectResources(fonts, images);
        for (const auto& name : images) {
            const auto dependency = findOutputEntity(current, name);
            if (dependency != entt::null) {
                result.push_back(dependency);
            }
        }
        return result;
    }
    if (!cameraOutputSize(current, entity).empty()) {
        for (const auto modelEntity : current.view<World::Model>()) {
            if (!Graphics::View::visible(current, modelEntity)) {
                continue;
            }
            for (const auto& material : current.get<World::Model>(modelEntity).materials()) {
                for (const auto& [slot, sourceEntity] : material.textures()) {
                    if (sourceEntity != entt::null && sourceEntity != entity && hasOutput(current, sourceEntity)) {
                        result.push_back(sourceEntity);
                    }
                }
            }
        }
    }
    return result;
}

std::vector<entt::entity> Local::textureOrder(const World::Scene& current, const std::vector<entt::entity>& roots) {
    std::map<entt::entity, int> states;
    std::vector<entt::entity> order;
    std::function<void(entt::entity)> visit = [&](entt::entity entity) {
        if (states[entity] == 1) {
            throw std::invalid_argument("Texture dependency cycle at: " + outputLabel(current, entity));
        }
        if (states[entity] == 2) {
            return;
        }
        if (!hasOutput(current, entity)) {
            return;
        }
        states[entity] = 1;
        for (const auto dependency : dependencies(current, entity)) {
            visit(dependency);
        }
        states[entity] = 2;
        order.push_back(entity);
    };
    for (const auto root : roots) {
        visit(root);
    }
    return order;
}

VOLCANO_FRAMEWORK_END
