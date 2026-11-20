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
#include <Volcano/World/PerspectiveCamera.h>
#include <Volcano/Framework/Drawing.h>
#include <Volcano/Framework/Local.h>
#include <Volcano/Framework/Resources.h>

VOLCANO_FRAMEWORK_BEGIN

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
    } catch (const async::task_canceled&) {
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

void Local::render(Graphics::Target& target, entt::entity camera) {
    window_.makeCurrent();
    std::vector<OutputKey> roots;
    for (const auto entity : scene().view<World::Model>()) {
        if (Graphics::View::visible(scene(), entity)) {
            for (const auto& material : scene().get<World::Model>(entity).materials()) {
                for (const auto& [slot, source] : material.textures()) {
                    if (source && source->isCanvas()) {
                        roots.push_back(OutputKey(source.get()));
                    }
                }
            }
        }
    }
    if (const auto* activeCamera = findCamera(scene(), camera);
        activeCamera != nullptr && activeCamera->hud() != nullptr &&
        activeCamera->hud()->isCanvas()) {
        roots.push_back(OutputKey(activeCamera->hud()));
    }
    for (const auto& [key, output] : outputs_) {
        if (std::holds_alternative<entt::entity>(key)) {
            const auto entity = std::get<entt::entity>(key);
            if (scene().valid(entity) && Graphics::View::visible(scene(), entity)) {
                roots.push_back(key);
            }
        }
    }
    updateTextures(roots, target.size());

    view_.extract(scene(), camera, [this](const World::Texture& source) {
        return resolveTexture(source);
    });
    renderer_.render(view_, target);
}

bool Local::pointer(entt::entity entity, const Gui::PointerEvent& event) {
    if (!scene().valid(entity)) {
        throw std::invalid_argument("Invalid GUI entity");
    }
    auto* doc = findDocument(scene(), entity);
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
    auto* doc = findDocument(scene(), entity);
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

Size Local::outputSize(const World::Scene& scene, entt::entity entity) {
    if (!scene.valid(entity)) {
        return {};
    }
    if (const auto* doc = scene.try_get<Gui::Document>(entity)) {
        return doc->size();
    }
    if (const auto* drawing = scene.try_get<Framework::Drawing>(entity)) {
        return drawing->size();
    }
    return {};
}

bool Local::hasOutput(const World::Scene& scene, entt::entity entity) {
    if (!scene.valid(entity)) {
        return false;
    }
    return scene.try_get<Gui::Document>(entity) != nullptr ||
        scene.try_get<Framework::Drawing>(entity) != nullptr;
}

bool Local::hasOutput(const World::Scene& scene, const OutputKey& key) {
    if (std::holds_alternative<entt::entity>(key)) {
        return hasOutput(scene, std::get<entt::entity>(key));
    }
    const auto* texture = std::get<const World::Texture*>(key);
    return texture != nullptr && texture->isCanvas() && containsTexture(scene, texture);
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
    for (const auto entity : scene.view<World::Inherent>()) {
        if (scene.get<World::Inherent>(entity).name() != name) {
            continue;
        }
        if (scene.try_get<Gui::Document>(entity) != nullptr) {
            return entity;
        }
        if (const auto* camera = findCamera(scene, entity); camera != nullptr && camera->hud() != nullptr &&
            camera->hud()->kind() == World::Texture::Kind::Document) {
            return entity;
        }
    }
    return entt::null;
}

void Local::onBeforeTasks() {
    window_.makeCurrent();
}

async::task<void> Local::onLoadScene(const World::Scene& candidate) {
    loading_.checkCancelled();
    pending_output_scene_ = &candidate;
    return Base::onLoadScene(candidate).then(scheduler(), [this, &candidate]() {
        loading_.checkCancelled();
        std::vector<OutputKey> keys;

        for (const auto entity : candidate.view<Gui::Document>()) {
            loading_.checkCancelled();
            const auto& doc = candidate.get<Gui::Document>(entity);
            loading_.setContent(outputLabel(candidate, entity));
            keys.push_back(entity);
            if (!doc.size().empty()) {
                loadOutput(entity, doc.size());
            }

            std::vector<std::filesystem::path> fonts;
            std::vector<std::string> images;
            doc.collectResources(fonts, images);
            for (const auto& path : fonts) {
                loading_.checkCancelled();
                loading_.setContent(path.generic_string());
                pending_fonts_.insert_or_assign(path.lexically_normal(),
                    Resources::loadFont(path.generic_string()));
            }
        }

        for (const auto entity : candidate.view<Framework::Drawing>()) {
            loading_.checkCancelled();
            const auto& drawing = candidate.get<Framework::Drawing>(entity);
            loading_.setContent(drawing.name());
            keys.push_back(entity);
            loadOutput(entity, drawing.size());

            for (const auto& path : drawing.fonts()) {
                loading_.checkCancelled();
                loading_.setContent(path.generic_string());
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
            if (const auto* hud = candidate.get<World::PerspectiveCamera>(entity).hud(); hud != nullptr) {
                loadTextureResource(*hud, keys);
            }
        }

        for (const auto entity : candidate.view<World::OrthographicCamera>()) {
            if (const auto* hud = candidate.get<World::OrthographicCamera>(entity).hud(); hud != nullptr) {
                loadTextureResource(*hud, keys);
            }
        }

        for (const auto entity : candidate.view<World::Model>()) {
            for (const auto& material : candidate.get<World::Model>(entity).materials()) {
                for (const auto& [slot, source] : material.textures()) {
                    if (source && source->kind() != World::Texture::Kind::Path) {
                        loadTextureResource(*source, keys);
                    }
                }
            }
        }

        loading_.checkCancelled();
        textureOrder(candidate, keys);
    });
}

async::task<void> Local::onLoadEntity(const World::Scene& candidate, entt::entity entity) {
    return Base::onLoadEntity(candidate, entity).then(scheduler(), [this, &candidate, entity]() {
        loading_.checkCancelled();
        const auto* model = candidate.try_get<World::Model>(entity);
        if (model == nullptr) {
            return;
        }
        const auto& parts = this->model(model->source()).parts();
        if (model->materials().size() > parts.size()) {
            throw std::invalid_argument("Model material override part index is out of range");
        }
        for (const auto& material : model->materials()) {
            for (const auto& [slot, source] : material.textures()) {
                if (source && source->kind() == World::Texture::Kind::Path) {
                    loading_.checkCancelled();
                    loading_.setContent(source->source().generic_string());
                    pending_textures_.insert_or_assign(
                        source->source().lexically_normal(),
                        Resources::loadTexture(source->source().generic_string()));
                }
            }
        }
        std::vector<Graphics::Model::Part> resolvedParts;
        resolvedParts.reserve(parts.size());
        for (const auto& part : parts) {
            loading_.checkCancelled();
            loading_.setContent(part.mesh());
            const auto meshPath = std::filesystem::path(part.mesh()).lexically_normal();
            auto mesh = Resources::loadMesh(part.mesh());
            Graphics::Model::Part resolvedPart;
            resolvedPart.mesh = mesh.get();
            pending_meshes_.insert_or_assign(meshPath, std::move(mesh));

            loading_.checkCancelled();
            loading_.setContent(part.material());
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
                loading_.checkCancelled();
                loading_.setContent(material.albedo_texture());
                auto albedo = Resources::loadTexture(material.albedo_texture());
                resolvedPart.material.texture = albedo.get();
                pending_textures_.insert_or_assign(
                    std::filesystem::path(material.albedo_texture()).lexically_normal(),
                    std::move(albedo));
            }
            for (const auto& path : {material.normal_texture(), material.metallic_roughness_texture()}) {
                loading_.checkCancelled();
                if (!path.empty()) {
                    loading_.setContent(path);
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
    });
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

const World::Camera* Local::findCamera(const World::Scene& scene, entt::entity entity) noexcept {
    if (!scene.valid(entity)) {
        return nullptr;
    }
    if (const auto* perspective = scene.try_get<World::PerspectiveCamera>(entity)) {
        return perspective;
    }
    if (const auto* orthographic = scene.try_get<World::OrthographicCamera>(entity)) {
        return orthographic;
    }
    return nullptr;
}

World::Camera* Local::findCamera(World::Scene& scene, entt::entity entity) noexcept {
    if (!scene.valid(entity)) {
        return nullptr;
    }
    if (auto* perspective = scene.try_get<World::PerspectiveCamera>(entity)) {
        return perspective;
    }
    if (auto* orthographic = scene.try_get<World::OrthographicCamera>(entity)) {
        return orthographic;
    }
    return nullptr;
}

Gui::Document* Local::findDocument(World::Scene& scene, entt::entity entity) noexcept {
    if (!scene.valid(entity)) {
        return nullptr;
    }
    if (auto* doc = scene.try_get<Gui::Document>(entity)) {
        return doc;
    }
    if (auto* camera = findCamera(scene, entity); camera != nullptr && camera->hud() != nullptr &&
        camera->hud()->kind() == World::Texture::Kind::Document) {
        return static_cast<Gui::Document*>(static_cast<World::Canvas*>(camera->hud()));
    }
    return nullptr;
}

bool Local::containsTexture(const World::Scene& scene, const World::Texture* texture) {
    if (texture == nullptr) {
        return false;
    }
    for (const auto entity : scene.view<World::PerspectiveCamera>()) {
        if (scene.get<World::PerspectiveCamera>(entity).hud() == texture) {
            return true;
        }
    }
    for (const auto entity : scene.view<World::OrthographicCamera>()) {
        if (scene.get<World::OrthographicCamera>(entity).hud() == texture) {
            return true;
        }
    }
    for (const auto entity : scene.view<World::Model>()) {
        for (const auto& material : scene.get<World::Model>(entity).materials()) {
            for (const auto& [slot, source] : material.textures()) {
                if (source.get() == texture) {
                    return true;
                }
            }
        }
    }
    return false;
}

std::string Local::outputLabel(const World::Scene& scene, const OutputKey& key) {
    if (std::holds_alternative<entt::entity>(key)) {
        const auto entity = std::get<entt::entity>(key);
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
    return "texture";
}

void Local::loadTextureResource(const World::Texture& texture, std::vector<OutputKey>& keys) {
    loading_.checkCancelled();
    if (texture.kind() == World::Texture::Kind::Path) {
        loading_.setContent(texture.source().generic_string());
        pending_textures_.insert_or_assign(
            texture.source().lexically_normal(),
            Resources::loadTexture(texture.source().generic_string()));
        return;
    }
    if (texture.isCanvas()) {
        const auto& canvas = static_cast<const World::Canvas&>(texture);
        if (!canvas.size().empty()) {
            loadOutput(&texture, canvas.size());
        }
        keys.push_back(&texture);

        std::vector<std::filesystem::path> fonts;
        std::vector<std::string> images;
        canvas.collectResources(fonts, images);
        for (const auto& path : fonts) {
            loading_.checkCancelled();
            loading_.setContent(path.generic_string());
            pending_fonts_.insert_or_assign(path.lexically_normal(),
                Resources::loadFont(path.generic_string()));
        }
    }
}

const Graphics::Texture* Local::resolveTexture(const World::Texture& source) const {
    if (source.kind() == World::Texture::Kind::Graphics) {
        return static_cast<const Graphics::Texture*>(&source);
    }
    if (source.kind() == World::Texture::Kind::Path) {
        const auto normal = source.source().lexically_normal();
        const auto found = textures_.find(normal);
        return found != textures_.end() ? found->second.get() : nullptr;
    }
    if (source.isCanvas()) {
        const auto found = outputs_.find(OutputKey(&source));
        return found != outputs_.end() ? &found->second->target.texture() : nullptr;
    }
    return nullptr;
}

void Local::updateTextures(const std::vector<OutputKey>& roots, Size defaultSize) {
    auto findMutableTexture = [&](const World::Texture* target) -> World::Texture* {
        for (const auto entity : scene().view<World::PerspectiveCamera>()) {
            auto* hud = scene().get<World::PerspectiveCamera>(entity).hud();
            if (hud == target) {
                return hud;
            }
        }
        for (const auto entity : scene().view<World::OrthographicCamera>()) {
            auto* hud = scene().get<World::OrthographicCamera>(entity).hud();
            if (hud == target) {
                return hud;
            }
        }
        for (const auto entity : scene().view<World::Model>()) {
            for (auto& material : scene().get<World::Model>(entity).materials()) {
                for (const auto& [slot, source] : material.textures()) {
                    if (source.get() == target) {
                        return source.get();
                    }
                }
            }
        }
        return nullptr;
    };

    std::set<OutputKey> updated;
    for (const auto& key : textureOrder(scene(), roots)) {
        bool dependencyChanged = false;
        for (const auto& dependency : dependencies(scene(), key)) {
            dependencyChanged = dependencyChanged || updated.contains(dependency);
        }

        World::Canvas* canvas = nullptr;
        const Framework::Drawing* drawing = nullptr;
        if (std::holds_alternative<entt::entity>(key)) {
            const auto entity = std::get<entt::entity>(key);
            canvas = scene().try_get<Gui::Document>(entity);
            drawing = scene().try_get<Framework::Drawing>(entity);
        } else {
            const auto* source = std::get<const World::Texture*>(key);
            if (source->isCanvas()) {
                canvas = static_cast<World::Canvas*>(findMutableTexture(source));
            }
        }

        if (canvas != nullptr) {
            const Size size = canvas->size().empty() ? defaultSize : canvas->size();
            if (size.width <= 0 || size.height <= 0) {
                continue;
            }
            if (!outputs_.contains(key)) {
                outputs_.emplace(key, std::make_unique<TextureOutput>(size));
            }
            auto& output = *outputs_.at(key);
            if (output.target.size() != size) {
                output.target.resize(size.width, size.height);
                output.dirty = true;
            }
            auto revision = canvas->revision();
            std::vector<std::filesystem::path> fontPaths;
            std::vector<std::string> imagePaths;
            canvas->collectResources(fontPaths, imagePaths);
            for (const auto& path : fontPaths) {
                if (const auto found = fonts_.find(path.lexically_normal()); found != fonts_.end()) {
                    revision += found->second->revision();
                }
            }
            if (!output.dirty && !dependencyChanged && output.revision == revision) {
                continue;
            }
            output.dirty = true;
            canvas->prepare({static_cast<float>(size.width), static_cast<float>(size.height)});
            if (!canvas->ready()) {
                continue;
            }
            output.revision = revision;
            renderer_.renderVG(*canvas, output.target);
            output.dirty = false;
            updated.insert(key);
            continue;
        }

        if (drawing != nullptr && outputs_.contains(key)) {
            auto& output = *outputs_.at(key);
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
            output.dirty = false;
            updated.insert(key);
        }
    }
}

Local::TextureOutput::TextureOutput(Size size) : target(size.width, size.height) {}

void Local::loadOutput(const OutputKey& key, Size size) {
    VOLCANO_ASSERT(pending_output_scene_ != nullptr);
    if ((output_scene_ != pending_output_scene_ || !outputs_.contains(key)) &&
        !pending_outputs_.contains(key)) {
        pending_outputs_.emplace(key, std::make_unique<TextureOutput>(size));
    }
}

void Local::validateSamples(const World::Canvas& canvas, const std::vector<std::string>& dependencies) const {
    for (const auto& cmd : canvas.commands()) {
        if (cmd.type == World::Canvas::Command::Type::Image && !cmd.texture.empty()) {
            const auto sampled = findOutputEntity(scene(), cmd.texture);
            if (sampled != entt::null && outputs_.contains(OutputKey(sampled)) &&
                std::find(dependencies.begin(), dependencies.end(), cmd.texture) == dependencies.end()) {
                throw std::invalid_argument("Drawing texture sampled an undeclared dependency: " + cmd.texture);
            }
        }
    }
}

std::vector<Local::OutputKey> Local::dependencies(const World::Scene& current, const OutputKey& key) {
    std::vector<OutputKey> result;
    const World::Canvas* canvas = nullptr;
    if (std::holds_alternative<entt::entity>(key)) {
        const auto entity = std::get<entt::entity>(key);
        if (const auto* drawing = current.try_get<Framework::Drawing>(entity)) {
            for (const auto& name : drawing->dependencies()) {
                const auto dependency = findOutputEntity(current, name);
                if (dependency != entt::null) {
                    result.push_back(dependency);
                }
            }
            return result;
        }
        canvas = current.try_get<Gui::Document>(entity);
    } else {
        const auto* texture = std::get<const World::Texture*>(key);
        if (texture != nullptr && texture->isCanvas()) {
            canvas = static_cast<const World::Canvas*>(texture);
        }
    }
    if (canvas != nullptr) {
        std::vector<std::filesystem::path> fonts;
        std::vector<std::string> images;
        canvas->collectResources(fonts, images);
        for (const auto& name : images) {
            const auto dependency = findOutputEntity(current, name);
            if (dependency != entt::null) {
                result.push_back(dependency);
            }
        }
    }
    return result;
}

std::vector<Local::OutputKey> Local::textureOrder(const World::Scene& current, const std::vector<OutputKey>& roots) {
    std::map<OutputKey, int> states;
    std::vector<OutputKey> order;
    std::function<void(const OutputKey&)> visit = [&](const OutputKey& key) {
        if (states[key] == 1) {
            throw std::invalid_argument("Texture dependency cycle at: " + outputLabel(current, key));
        }
        if (states[key] == 2) {
            return;
        }
        if (!hasOutput(current, key)) {
            return;
        }
        states[key] = 1;
        for (const auto& dependency : dependencies(current, key)) {
            visit(dependency);
        }
        states[key] = 2;
        order.push_back(key);
    };
    for (const auto& root : roots) {
        visit(root);
    }
    return order;
}

VOLCANO_FRAMEWORK_END
