#include <algorithm>
#include <filesystem>
#include <limits>
#include <set>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <Eigen/LU>

#include <Volcano/Transform.h>
#include <Volcano/Gui/Document.h>
#include <Volcano/World/Canvas.h>
#include <Volcano/World/DirectionalLight.h>
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Model.h>
#include <Volcano/World/OrthographicCamera.h>
#include <Volcano/World/PerspectiveCamera.h>
#include <Volcano/World/PointLight.h>
#include <Volcano/World/SpotLight.h>
#include <Volcano/Framework/Drawing.h>
#include <Volcano/Framework/Local.h>
#include <Volcano/Framework/Resources.h>

VOLCANO_FRAMEWORK_BEGIN

Eigen::Affine3f Local::entityTransform(const World::Scene& scene, entt::entity entity) {
    const auto* transform = scene.try_get<Transform>(entity);
    return transform != nullptr ? transform->affine() : Eigen::Affine3f::Identity();
}

bool Local::visible(const World::Scene& scene, entt::entity entity) {
    const auto* inherent = scene.try_get<World::Inherent>(entity);
    return inherent == nullptr || (inherent->isEnabled() && inherent->isVisible());
}

bool Local::hasOutput(const World::Scene& scene, std::string_view name) {
    return findNamed<Gui::Document>(scene, name) != nullptr ||
           findNamed<Framework::Drawing>(scene, name) != nullptr;
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

const Graphics::Texture& Local::texture(std::string_view id) const {
    window_.makeCurrent();
    const std::string key(id);
    if (const auto found = outputs_.find(key); found != outputs_.end()) {
        return found->second->target.texture();
    }
    const auto normal = std::filesystem::path(id).lexically_normal();
    if (const auto found = textures_.find(normal); found != textures_.end()) {
        return *found->second;
    }
    throw std::out_of_range("Texture not found: " + key);
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
    std::vector<std::string> ids;

    for (const auto entity : candidate.view<World::Inherent, Gui::Document>()) {
        checkLoadCancelled();
        const auto& inherent = candidate.get<World::Inherent>(entity);
        const auto& doc = candidate.get<Gui::Document>(entity);
        setLoadingContent(inherent.name());
        ids.push_back(inherent.name());
        loadOutput(inherent.name(), doc.size());

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
        ids.push_back(drawing.name());
        loadOutput(drawing.name(), drawing.size());

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
    for (const auto& [materialPath, material] : model->materials()) {
        for (const auto& [slot, sourceEntity] : material.textures()) {
            const bool isOutput = candidate.valid(sourceEntity) &&
                                  (candidate.try_get<Gui::Document>(sourceEntity) != nullptr ||
                                   candidate.try_get<Framework::Drawing>(sourceEntity) != nullptr ||
                                   (candidate.try_get<World::Inherent>(sourceEntity) != nullptr &&
                                    hasOutput(candidate, candidate.get<World::Inherent>(sourceEntity).name())));
            if (!isOutput ||
                std::none_of(parts.begin(), parts.end(), [&](const auto& part) {
                    return part.material() == materialPath;
                })) {
                throw std::invalid_argument(
                    "Model material texture override must reference an existing material and entity");
            }
        }
    }
    for (const auto& part : parts) {
        checkLoadCancelled();
        setLoadingContent(part.mesh());
        const auto meshPath = std::filesystem::path(part.mesh()).lexically_normal();
        pending_meshes_.insert_or_assign(meshPath,
            Resources::loadMesh(part.mesh()));
        checkLoadCancelled();
        setLoadingContent(part.material());
        auto resource = Resources::loadMaterial(part.material());
        const auto& material = *resource;
        const auto materialPath = std::filesystem::path(part.material()).lexically_normal();
        pending_materials_.insert_or_assign(materialPath, std::move(resource));
        for (const auto& path :
             {material.albedo_texture(), material.normal_texture(), material.metallic_roughness_texture()}) {
            checkLoadCancelled();
            if (!path.empty()) {
                setLoadingContent(path);
                pending_textures_.insert_or_assign(std::filesystem::path(path).lexically_normal(),
                    Resources::loadTexture(path));
            }
        }
    }
}

void Local::onCommitResources() noexcept {
    view_.begin();
    if (output_scene_ != pending_output_scene_) {
        outputs_.swap(pending_outputs_);
        pending_outputs_.clear();
    } else {
        outputs_.merge(pending_outputs_);
    }
    for (auto& [id, output] : outputs_) {
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
}

void Local::onDiscardResources() noexcept {
    pending_fonts_.clear();
    pending_materials_.clear();
    pending_meshes_.clear();
    pending_textures_.clear();
    pending_outputs_.clear();
    pending_output_scene_ = nullptr;
}

void Local::onUnloadResources() noexcept {
    window_.makeCurrent();
    view_.begin();
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

void Local::buildView(Graphics::View& view, entt::entity cameraEntity) {
    window_.makeCurrent();
    const auto& current = scene();
    VOLCANO_ASSERT(current.valid(cameraEntity));
    const auto* perspective = current.try_get<World::PerspectiveCamera>(cameraEntity);
    const auto* orthographic = current.try_get<World::OrthographicCamera>(cameraEntity);
    if ((perspective == nullptr) == (orthographic == nullptr)) {
        throw std::invalid_argument("Camera entity requires exactly one supported camera component");
    }
    const World::Camera& camera = perspective != nullptr ? static_cast<const World::Camera&>(*perspective) : *orthographic;
    const Eigen::Affine3f cameraTransform = entityTransform(current, cameraEntity) * camera.offset().affine();
    if (!cameraTransform.matrix().allFinite() || !cameraTransform.linear().fullPivLu().isInvertible()) {
        throw std::invalid_argument("Camera transform must be finite and invertible");
    }

    view.begin();
    view.setProjection(camera.toMatrix());
    view.setViewMatrix(cameraTransform.inverse().matrix());
    for (const auto entity : current.view<World::Model>()) {
        if (!visible(current, entity)) {
            continue;
        }
        const auto transform = entityTransform(current, entity);
        for (const auto& part : this->model(current.get<World::Model>(entity).source()).parts()) {
            const auto partMaterialPath = std::filesystem::path(part.material()).lexically_normal();
            const auto partMeshPath = std::filesystem::path(part.mesh()).lexically_normal();
            const auto& source = *materials_.at(partMaterialPath);
            Graphics::Material material;
            if (source.has_albedo()) {
                material.color = Colorf(source.albedo().x(), source.albedo().y(), source.albedo().z(), source.albedo().w());
            } else {
                material.color = Colorf::White;
            }
            material.lit = source.has_lit() ? source.lit() : true;
            if (!source.albedo_texture().empty()) {
                material.texture = textures_.at(std::filesystem::path(source.albedo_texture()).lexically_normal()).get();
            }
            if (const auto* customMaterial = current.get<World::Model>(entity).findMaterial(part.material());
                customMaterial != nullptr) {
                auto resolveSource = [&](World::Material::Slot slot) -> const Graphics::Texture* {
                    const auto sourceEntity = customMaterial->texture(slot);
                    if (sourceEntity != entt::null && current.valid(sourceEntity)) {
                        if (const auto* inherent = current.try_get<World::Inherent>(sourceEntity)) {
                            return &texture(inherent->name());
                        }
                        if (const auto* drawing = current.try_get<Framework::Drawing>(sourceEntity)) {
                            return &texture(drawing->name());
                        }
                    }
                    return nullptr;
                };

                if (const auto* tex = resolveSource(World::Material::Slot::Albedo); tex != nullptr) {
                    material.texture = tex;
                } else if (const auto* tex = resolveSource(World::Material::Slot::Emissive); tex != nullptr) {
                    material.texture = tex;
                }
            }
            Eigen::Affine3f partTransform = Eigen::Affine3f::Identity();
            if (part.transform_size() == 16) {
                for (int r = 0; r < 4; ++r) {
                    for (int c = 0; c < 4; ++c) {
                        partTransform.matrix()(r, c) = part.transform(r * 4 + c);
                    }
                }
            }
            const Eigen::Affine3f model = transform * partTransform;
            // shortcut: Graphics currently uses albedo and lit; add PBR maps when its passes support them.
            view.add(*meshes_.at(partMeshPath), material, model, source.alpha_mode() == Asset::ALPHA_MODE_BLEND);
        }
    }

    auto addLights = [&]<typename Component>() {
        for (const auto entity : current.view<Component>()) {
            if (!visible(current, entity)) {
                continue;
            }
            const auto& source = current.get<Component>(entity);
            const Eigen::Affine3f transform = entityTransform(current, entity) * source.offset().affine();
            Graphics::Light light;
            light.color = source.color();
            light.strength = source.strength();
            light.position = transform.translation();
            if constexpr (std::is_same_v<Component, World::PointLight>) {
                light.type = Graphics::Light::Type::Point;
                light.range = source.range();
            } else {
                light.direction = transform.linear() * source.direction();
                if constexpr (std::is_same_v<Component, World::SpotLight>) {
                    light.type = Graphics::Light::Type::Spot;
                    light.range = source.range();
                    light.innerAngle = source.innerAngle();
                    light.outerAngle = source.outerAngle();
                }
            }
            view.add(light);
        }
    };
    addLights.operator()<World::DirectionalLight>();
    addLights.operator()<World::PointLight>();
    addLights.operator()<World::SpotLight>();

}

void Local::updateTextures(const std::vector<std::string>& roots) {
    std::set<std::string> updated;
    for (const auto& id : textureOrder(scene(), roots)) {
        if (!outputs_.contains(id)) {
            continue;
        }
        auto& output = *outputs_.at(id);
        bool dependencyChanged = false;
        for (const auto& dependency : dependencies(scene(), id)) {
            dependencyChanged = dependencyChanged || updated.contains(dependency);
        }
        if (auto* doc = findNamed<Gui::Document>(scene(), id)) {
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
        } else if (const auto* drawing = findNamed<Framework::Drawing>(scene(), id)) {
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
        }
        output.dirty = false;
        updated.insert(id);
    }
}

void Local::render(Graphics::Target& target, entt::entity camera) {
    window_.makeCurrent();
    std::vector<std::string> roots;
    for (const auto entity : scene().view<World::Model>()) {
        if (visible(scene(), entity)) {
            for (const auto& [materialPath, material] : scene().get<World::Model>(entity).materials()) {
                for (const auto& [slot, sourceEntity] : material.textures()) {
                    if (scene().valid(sourceEntity)) {
                        if (const auto* inherent = scene().try_get<World::Inherent>(sourceEntity)) {
                            roots.push_back(inherent->name());
                        } else if (const auto* drawing = scene().try_get<Framework::Drawing>(sourceEntity)) {
                            roots.push_back(drawing->name());
                        }
                    }
                }
            }
        }
    }
    for (const auto entity : scene().view<World::Inherent, Gui::Document>()) {
        if (visible(scene(), entity)) {
            roots.push_back(scene().get<World::Inherent>(entity).name());
        }
    }
    for (const auto entity : scene().view<Framework::Drawing>()) {
        if (visible(scene(), entity)) {
            roots.push_back(scene().get<Framework::Drawing>(entity).name());
        }
    }
    updateTextures(roots);
    buildView(view_, camera);
    view_.end();
    renderer_.render(view_, target);
}

bool Local::pointer(std::string_view id, const Gui::PointerEvent& event) {
    auto* doc = findNamed<Gui::Document>(scene(), id);
    if (doc == nullptr) {
        throw std::invalid_argument("Texture is not a GUI texture: " + std::string(id));
    }
    const auto size = doc->size();
    doc->layout({static_cast<float>(size.width), static_cast<float>(size.height)});
    return doc->event(event);
}

bool Local::key(std::string_view id, const Gui::KeyEvent& event) {
    auto* doc = findNamed<Gui::Document>(scene(), id);
    if (doc == nullptr) {
        throw std::invalid_argument("Texture is not a GUI texture: " + std::string(id));
    }
    return doc->event(event);
}

Local::TextureOutput::TextureOutput(Size size) : target(size.width, size.height) {}

void Local::loadOutput(std::string id, Size size) {
    VOLCANO_ASSERT(pending_output_scene_ != nullptr);
    if ((output_scene_ != pending_output_scene_ || !outputs_.contains(id)) &&
        !pending_outputs_.contains(id)) {
        pending_outputs_.emplace(std::move(id), std::make_unique<TextureOutput>(size));
    }
}

void Local::validateSamples(const World::Canvas& canvas, const std::vector<std::string>& dependencies) const {
    for (const auto& cmd : canvas.commands()) {
        if (cmd.type == World::Canvas::Command::Type::Image && !cmd.texture.empty()) {
            if (outputs_.contains(cmd.texture) &&
                std::find(dependencies.begin(), dependencies.end(), cmd.texture) == dependencies.end()) {
                throw std::invalid_argument("Drawing texture sampled an undeclared dependency: " + cmd.texture);
            }
        }
    }
}

std::vector<std::string> Local::dependencies(const World::Scene& current, const std::string& id) {
    if (const auto* drawing = findNamed<Framework::Drawing>(current, id)) {
        return drawing->dependencies();
    }
    std::vector<std::string> result;
    if (const auto* doc = findNamed<Gui::Document>(current, id)) {
        std::vector<std::filesystem::path> fonts;
        doc->root().collectResources(fonts, result);
    }
    return result;
}

std::vector<std::string> Local::textureOrder(const World::Scene& current, const std::vector<std::string>& roots) {
    std::map<std::string, int> states;
    std::vector<std::string> order;
    std::function<void(const std::string&)> visit = [&](const std::string& id) {
        if (states[id] == 1) {
            throw std::invalid_argument("Texture dependency cycle at: " + id);
        }
        if (states[id] == 2) {
            return;
        }
        if (!hasOutput(current, id)) {
            return;
        }
        states[id] = 1;
        for (const auto& dependency : dependencies(current, id)) {
            visit(dependency);
        }
        states[id] = 2;
        order.push_back(id);
    };
    for (const auto& root : roots) {
        visit(root);
    }
    return order;
}

VOLCANO_FRAMEWORK_END
