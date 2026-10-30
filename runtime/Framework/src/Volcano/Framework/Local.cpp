#include <limits>
#include <algorithm>
#include <filesystem>
#include <filesystem>
#include <set>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <Eigen/LU>

#include <Volcano/Transform.h>
#include <Volcano/World/DirectionalLight.h>
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Model.h>
#include <Volcano/World/Hud.h>
#include <Volcano/World/OrthographicCamera.h>
#include <Volcano/World/PerspectiveCamera.h>
#include <Volcano/World/PointLight.h>
#include <Volcano/World/SpotLight.h>
#include <Volcano/Framework/GuiContext.h>
#include <Volcano/Framework/Local.h>
#include <Volcano/Framework/Resources.h>

VOLCANO_FRAMEWORK_BEGIN

namespace {
Eigen::Affine3f entityTransform(const World::Scene& scene, entt::entity entity) {
    const auto* transform = scene.try_get<Transform>(entity);
    return transform != nullptr ? transform->affine() : Eigen::Affine3f::Identity();
}

bool visible(const World::Scene& scene, entt::entity entity) {
    const auto* inherent = scene.try_get<World::Inherent>(entity);
    return inherent == nullptr || (inherent->isEnabled() && inherent->isVisible());
}
}

Local::Local(int width, int height, const char* title)
    : window_(width, height, title) {
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

const Graphics::Texture& Local::texture(std::string_view id) const {
    window_.makeCurrent();
    const std::string key(id);
    const auto& description = scene().texture(key);
    if (description.source() == World::Texture::Source::File) {
        return *textures_.at(static_cast<const World::FileTexture&>(description).path().lexically_normal());
    }
    return outputs_.at(key)->target.texture();
}

void Local::setDrawingProvider(std::string id, std::function<void(Graphics::Canvas&)> provider) {
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

async::task<void> Local::onLoadScene(const World::Scene& candidate) {
    checkLoadCancelled();
    pending_output_scene_ = &candidate;
    return Base::onLoadScene(candidate).then(scheduler(), [this, &candidate] {
        std::vector<std::string> ids;
        for (const auto& [id, description] : candidate.textures()) {
            checkLoadCancelled();
            setLoadingContent(id);
            ids.push_back(id);
            if (description->source() == World::Texture::Source::File) {
                const auto& file = static_cast<const World::FileTexture&>(*description);
                pending_textures_.insert_or_assign(file.path().lexically_normal(),
                    Resources::loadTexture(file.path().generic_string()));
            } else {
                const auto& generated = static_cast<const World::GeneratedTexture&>(*description);
                loadOutput(id, generated.size());
                if (description->source() == World::Texture::Source::Gui) {
                    const auto& gui = static_cast<const World::GuiTexture&>(*description);
                    std::vector<std::filesystem::path> fonts;
                    std::vector<std::string> images;
                    gui.document().root().collectResources(fonts, images);
                    for (const auto& path : fonts) {
                        checkLoadCancelled();
                        setLoadingContent(path.generic_string());
                        pending_fonts_.insert_or_assign(path.lexically_normal(),
                            Resources::loadFont(path.generic_string()));
                    }
                }
                if (description->source() == World::Texture::Source::Drawing) {
                    const auto& drawing = static_cast<const World::DrawingTexture&>(*description);
                    for (const auto& path : drawing.fonts()) {
                        checkLoadCancelled();
                        setLoadingContent(path.generic_string());
                        pending_fonts_.insert_or_assign(path.lexically_normal(),
                            Resources::loadFont(path.generic_string()));
                    }
                    // Querying the provider also validates its existence before committing.
                    try {
                        drawing_providers_.at(drawing.provider());
                    } catch (const std::out_of_range&) {
                        throw std::invalid_argument("Missing drawing provider: " + drawing.provider());
                    }
                }
            }
        }
        checkLoadCancelled();
        textureOrder(candidate, ids);
        for (const auto entity : candidate.view<World::Hud>()) {
            if (!candidate.textures().contains(candidate.get<World::Hud>(entity).texture())) {
                throw std::invalid_argument("HUD references an unknown texture");
            }
        }
    });
}

async::task<void> Local::onLoadEntity(const World::Scene& candidate, entt::entity entity) {
    return Base::onLoadEntity(candidate, entity).then(scheduler(), [this, &candidate, entity] {
        checkLoadCancelled();
        const auto* model = candidate.try_get<World::Model>(entity);
        if (model == nullptr) {
            return;
        }
        const auto& parts = this->model(model->source()).parts;
        for (const auto& [material, id] : model->textureOverrides()) {
            if (!candidate.textures().contains(id) ||
                std::none_of(parts.begin(), parts.end(), [&](const auto& part) {
                    return part.material == material;
                })) {
                throw std::invalid_argument(
                    "Model texture override must reference an existing material and texture");
            }
        }
        for (const auto& part : parts) {
            checkLoadCancelled();
            setLoadingContent(part.mesh.generic_string());
            pending_meshes_.insert_or_assign(part.mesh.lexically_normal(),
                Resources::loadMesh(part.mesh.generic_string()));
            checkLoadCancelled();
            setLoadingContent(part.material.generic_string());
            auto resource = Resources::loadMaterial(part.material.generic_string());
            const auto& material = *resource;
            pending_materials_.insert_or_assign(part.material.lexically_normal(), std::move(resource));
            for (const auto& path :
                 {material.albedoTexture, material.normalTexture, material.metallicRoughnessTexture}) {
                checkLoadCancelled();
                if (!path.empty()) {
                    setLoadingContent(path.generic_string());
                    pending_textures_.insert_or_assign(path.lexically_normal(),
                        Resources::loadTexture(path.generic_string()));
                }
            }
        }
    });
}

void Local::onCommitResources() noexcept {
    view_.begin();
    if (output_scene_ != pending_output_scene_) {
        pointer_gui_.clear();
        focused_gui_.clear();
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
    pointer_gui_.clear();
    focused_gui_.clear();
    hud_.begin();
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
    updateGuiInput();
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
        for (const auto& part : this->model(current.get<World::Model>(entity).source()).parts) {
            const auto& source = *materials_.at(part.material.lexically_normal());
            Graphics::Material material;
            material.color = Colorf(source.albedo.x(), source.albedo.y(), source.albedo.z(), source.albedo.w());
            material.lit = source.lit;
            if (!source.albedoTexture.empty()) {
                material.texture = textures_.at(source.albedoTexture.lexically_normal()).get();
            }
            const auto& overrides = current.get<World::Model>(entity).textureOverrides();
            if (const auto it = overrides.find(part.material); it != overrides.end()) {
                material.texture = &texture(it->second);
            }
            const Eigen::Affine3f model = transform * part.transform;
            // shortcut: Graphics currently uses albedo and lit; add PBR maps when its passes support them.
            view.add(*meshes_.at(part.mesh.lexically_normal()), material, model, source.alphaMode == Schema::Material::AlphaMode::Blend);
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
    for (const auto& id : textureOrder(scene(),roots)) {
        auto& description = scene().texture(id);
        if (description.source() == World::Texture::Source::File) {
            continue;
        }
        auto& output = *outputs_.at(id);
        const auto size = static_cast<World::GeneratedTexture&>(description).size();
        if (output.target.size() != size) {
            output.target.resize(size.width,size.height);
            output.dirty = true;
        }
        bool dependencyChanged = false;
        for (const auto& dependency : dependencies(scene(),id)) {
            dependencyChanged = dependencyChanged || updated.contains(dependency);
        }
        if (description.source() == World::Texture::Source::Camera) {
            const auto& camera = static_cast<const World::CameraTexture&>(description);
            Graphics::View view;
            buildView(view,camera.camera());
            view.end();
            renderer_.render(view,output.target);
        } else {
            if (description.source() == World::Texture::Source::Gui) {
                auto& gui = static_cast<World::GuiTexture&>(description);
                auto& document = gui.document();
                auto revision = document.revision();
                std::vector<std::filesystem::path> fontPaths;
                std::vector<std::string> imagePaths;
                document.root().collectResources(fontPaths,imagePaths);
                for (const auto& path : fontPaths) {
                    revision += fonts_.at(path.lexically_normal())->revision();
                }
                if (!output.dirty && !dependencyChanged && output.revision == revision) {
                    continue;
                }
                output.dirty = true;
                document.layout({static_cast<float>(size.width),static_cast<float>(size.height)});
                output.canvas.begin();
                GuiContext context(output.canvas,[this](const auto& path) -> const Graphics::Font& { return *fonts_.at(path.lexically_normal()); },
                    [this](auto name) -> const Graphics::Texture& { return texture(name); });
                document.draw(context);
                output.canvas.end();
                output.revision = revision;
            } else {
                VOLCANO_ASSERT(description.source() == World::Texture::Source::Drawing);
                const auto& drawing = static_cast<const World::DrawingTexture&>(description);
                output.canvas.begin();
                drawing_providers_.at(drawing.provider())(output.canvas);
                output.canvas.end();
                validateSamples(output.canvas, drawing.dependencies());
            }
            // A VG-only submission does not need a 3D clear or G-buffer allocation.
            renderer_.renderVG(output.canvas,output.target);
        }
        output.dirty = false;
        updated.insert(id);
    }
}

void Local::render(Graphics::Target& target, entt::entity camera) {
    window_.makeCurrent();
    std::vector<std::string> roots;
    for (const auto entity : scene().view<World::Model>()) {
        if (visible(scene(),entity)) {
            for (const auto& [material,id] : scene().get<World::Model>(entity).textureOverrides()) {
                roots.push_back(id);
            }
        }
    }
    std::vector<entt::entity> huds;
    for (const auto entity : scene().view<World::Hud>()) {
        if (visible(scene(),entity)) {
            huds.push_back(entity);
            roots.push_back(scene().get<World::Hud>(entity).texture());
        }
    }
    updateTextures(roots);
    buildView(view_,camera);
    std::stable_sort(huds.begin(),huds.end(),[this](auto a,auto b) {
        return scene().get<World::Hud>(a).order() < scene().get<World::Hud>(b).order();
    });
    hud_.begin();
    for (const auto entity : huds) {
        const auto& hud = scene().get<World::Hud>(entity);
        hud_.image(texture(hud.texture()),hud.rect());
    }
    hud_.end();
    view_.add(hud_);
    view_.end();
    renderer_.render(view_,target);
}

bool Local::pointer(std::string_view id, const Gui::PointerEvent& event) {
    auto& texture = scene().texture(std::string(id));
    if (texture.source() != World::Texture::Source::Gui) {
        throw std::invalid_argument("Texture is not a GUI texture: " + std::string(id));
    }
    auto& description = static_cast<World::GuiTexture&>(texture);
    const auto size = description.size();
    description.document().layout({static_cast<float>(size.width),static_cast<float>(size.height)});
    return description.document().event(event);
}

bool Local::key(std::string_view id, const Gui::KeyEvent& event) {
    auto& texture = scene().texture(std::string(id));
    if (texture.source() != World::Texture::Source::Gui) {
        throw std::invalid_argument("Texture is not a GUI texture: " + std::string(id));
    }
    auto& description = static_cast<World::GuiTexture&>(texture);
    return description.document().event(event);
}

void Local::updateGuiInput() {
    int windowWidth = 0, windowHeight = 0;
    glfwGetWindowSize(window_.handle(),&windowWidth,&windowHeight);
    if (windowWidth <= 0 || windowHeight <= 0) {
        return;
    }
    double cursorX = 0, cursorY = 0;
    glfwGetCursorPos(window_.handle(),&cursorX,&cursorY);
    const float x = static_cast<float>(cursorX)*window_.width()/windowWidth;
    const float y = static_cast<float>(cursorY)*window_.height()/windowHeight;
    const bool down = glfwGetMouseButton(window_.handle(),GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS;
    std::string hovered;
    Rectf rect;
    int highest = std::numeric_limits<int>::lowest();
    for (const auto entity : scene().view<World::Hud>()) {
        const auto& hud = scene().get<World::Hud>(entity);
        if (visible(scene(),entity) && hud.rect().contains(x,y) && hud.order() >= highest &&
            scene().textures().contains(hud.texture()) &&
            scene().texture(hud.texture()).source()==World::Texture::Source::Gui) {
            highest = hud.order();
            hovered = hud.texture();
            rect = hud.rect();
        }
    }
    if (down && !mouse_down_) {
        pointer_gui_ = hovered;
        pointer_rect_ = rect;
        focused_gui_ = hovered;
    }
    const auto& id = pointer_gui_.empty() ? hovered : pointer_gui_;
    const auto& destination = pointer_gui_.empty() ? rect : pointer_rect_;
    if (!id.empty() && destination.width > 0 && destination.height > 0) {
        const auto& texture = scene().texture(id);
        if (texture.source() != World::Texture::Source::Gui) {
            throw std::invalid_argument("Texture is not a GUI texture: " + id);
        }
        const auto size = static_cast<const World::GuiTexture&>(texture).size();
        const float gx = (x-destination.left)*size.width/destination.width;
        const float gy = (y-destination.top)*size.height/destination.height;
        const auto event = down != mouse_down_ ? (down ? Gui::PointerEvent::Action::Down : Gui::PointerEvent::Action::Up)
            : Gui::PointerEvent::Action::Move;
        pointer(id,Gui::PointerEvent{event,gx,gy});
    }
    if (!down) {
        pointer_gui_.clear();
    }
    mouse_down_ = down;
    const bool enter = glfwGetKey(window_.handle(),GLFW_KEY_ENTER)==GLFW_PRESS;
    const bool space = glfwGetKey(window_.handle(),GLFW_KEY_SPACE)==GLFW_PRESS;
    if (!focused_gui_.empty()) {
        if (enter && !enter_down_) {
            key(focused_gui_,Gui::KeyEvent{Gui::KeyEvent::Action::Down,Gui::KeyEvent::Key::Enter});
        }
        if (space && !space_down_) {
            key(focused_gui_,Gui::KeyEvent{Gui::KeyEvent::Action::Down,Gui::KeyEvent::Key::Space});
        }
    }
    enter_down_ = enter;
    space_down_ = space;
}

Local::TextureOutput::TextureOutput(Size size) : target(size.width, size.height) {}

void Local::loadOutput(std::string id, Size size) {
    VOLCANO_ASSERT(pending_output_scene_ != nullptr);
    if ((output_scene_ != pending_output_scene_ || !outputs_.contains(id)) &&
        !pending_outputs_.contains(id)) {
        pending_outputs_.emplace(std::move(id), std::make_unique<TextureOutput>(size));
    }
}

void Local::validateSamples(const Graphics::Canvas& canvas, const std::vector<std::string>& dependencies) const {
    for (const auto* sampled : canvas.sampledTextures()) {
        for (const auto& [id, resource] : outputs_) {
            if (sampled == &resource->target.texture() &&
                std::find(dependencies.begin(), dependencies.end(), id) == dependencies.end()) {
                throw std::invalid_argument("Drawing texture sampled an undeclared dependency: " + id);
            }
        }
    }
}

std::vector<std::string> Local::dependencies(const World::Scene& current, const std::string& id) {
    const auto& description = current.texture(id);
    if (description.source() == World::Texture::Source::Drawing) {
        return static_cast<const World::DrawingTexture&>(description).dependencies();
    }
    std::vector<std::string> result;
    if (description.source() == World::Texture::Source::Gui) {
        const auto& gui = static_cast<const World::GuiTexture&>(description);
        std::vector<std::filesystem::path> fonts;
        gui.document().root().collectResources(fonts, result);
    } else if (description.source() == World::Texture::Source::Camera) {
        const auto& camera = static_cast<const World::CameraTexture&>(description);
        if (!current.valid(camera.camera()) ||
            (current.all_of<World::PerspectiveCamera>(camera.camera()) ==
             current.all_of<World::OrthographicCamera>(camera.camera()))) {
            throw std::invalid_argument("Texture camera requires exactly one camera component");
        }
        for (const auto entity : current.view<World::Model>()) {
            if (visible(current, entity)) {
                for (const auto& [material, texture] : current.get<World::Model>(entity).textureOverrides()) {
                    result.push_back(texture);
                }
            }
        }
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
        if (!current.textures().contains(id)) {
            throw std::invalid_argument("Unknown texture resource: " + id);
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
