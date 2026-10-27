#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <Eigen/LU>

#include <Volcano/Schema/Mesh.h>
#include <Volcano/Schema/Texture.h>
#include <Volcano/Transform.h>
#include <Volcano/World/DirectionalLight.h>
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Model.h>
#include <Volcano/World/OrthographicCamera.h>
#include <Volcano/World/PerspectiveCamera.h>
#include <Volcano/World/PointLight.h>
#include <Volcano/World/SpotLight.h>
#include <Volcano/Framework/Local.h>

#include "Resource.h"

VOLCANO_FRAMEWORK_BEGIN

namespace {
ScopeGuard<void (*)()> initializeGlfw() {
    if (!glfwInit()) {
        const char* description = nullptr;
        glfwGetError(&description);
        throw std::runtime_error(std::string("Cannot initialize GLFW: ") +
            (description != nullptr ? description : "unknown error"));
    }
    // shortcut: Local owns GLFW's process lifetime; share ownership before supporting concurrent Local instances.
    return scopeGuard(&glfwTerminate);
}

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
    : glfw_runtime_(initializeGlfw())
    , window_(width, height, title)
    , renderer_(glfwGetProcAddress) {
}

Local::~Local() {
    glfwMakeContextCurrent(window_.handle());
    unload();
}

Window& Local::window() noexcept {
    return window_;
}

const Window& Local::window() const noexcept {
    return window_;
}

void Local::render(entt::entity camera) {
    render(window_, camera);
}

Graphics::Renderer& Local::renderer() noexcept {
    return renderer_;
}

const Graphics::Renderer& Local::renderer() const noexcept {
    return renderer_;
}

Graphics::View& Local::view() noexcept {
    return view_;
}

const Graphics::View& Local::view() const noexcept {
    return view_;
}

void Local::loadMesh(const std::filesystem::path& path) {
    if (meshes_.contains(path) || pending_meshes_.contains(path)) {
        return;
    }
    const auto source = readResource<Schema::Mesh>(path);
    std::vector<Graphics::Mesh::Vertex> vertices;
    vertices.reserve(source.vertices.size());
    for (const auto& vertex : source.vertices) {
        vertices.push_back({vertex.position, vertex.normal, vertex.texcoord});
    }
    pending_meshes_.emplace(path, std::make_unique<Graphics::Mesh>(vertices, source.indices));
}

const Schema::Material& Local::loadMaterial(const std::filesystem::path& path) {
    if (const auto it = pending_materials_.find(path); it != pending_materials_.end()) {
        return it->second;
    }
    if (const auto it = materials_.find(path); it != materials_.end()) {
        return it->second;
    }
    auto material = readResource<Schema::Material>(path);
    for (auto* texture : {&material.albedoTexture, &material.normalTexture, &material.metallicRoughnessTexture}) {
        if (!texture->empty()) {
            *texture = resourcePath(path.parent_path() / *texture);
        }
    }
    return pending_materials_.emplace(path, std::move(material)).first->second;
}

void Local::loadTexture(const std::filesystem::path& path) {
    if (path.empty() || textures_.contains(path) || pending_textures_.contains(path)) {
        return;
    }
    const auto source = readResource<Schema::Texture>(path);
    if (source.width > std::numeric_limits<int>::max() || source.height > std::numeric_limits<int>::max()) {
        throw std::invalid_argument("Texture dimensions exceed integer range: " + path.generic_string());
    }
    auto texture = std::make_unique<Graphics::Texture>(static_cast<int>(source.width), static_cast<int>(source.height));
    texture->uploadPremultiplied(source.pixels);
    pending_textures_.emplace(path, std::move(texture));
}

void Local::onLoadResources(const World::Scene& candidate) {
    window_.makeCurrent();
    Base::onLoadResources(candidate);
    for (const auto entity : candidate.view<World::Model>()) {
        for (const auto& part : modelResource(candidate.get<World::Model>(entity).source).parts) {
            loadMesh(part.mesh);
            const auto& material = loadMaterial(part.material);
            loadTexture(material.albedoTexture);
            loadTexture(material.normalTexture);
            loadTexture(material.metallicRoughnessTexture);
        }
    }
}

void Local::onCommitResources() noexcept {
    materials_.merge(pending_materials_);
    meshes_.merge(pending_meshes_);
    textures_.merge(pending_textures_);
}

void Local::onDiscardResources() noexcept {
    pending_materials_.clear();
    pending_meshes_.clear();
    pending_textures_.clear();
}

void Local::onUnloadResources() noexcept {
    glfwMakeContextCurrent(window_.handle());
    view_.begin();
    materials_.clear();
    meshes_.clear();
    textures_.clear();
    onDiscardResources();
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

void Local::render(Graphics::Target& target, entt::entity cameraEntity) {
    window_.makeCurrent();
    const auto& current = scene();
    if (!current.valid(cameraEntity)) {
        throw std::invalid_argument("Camera entity does not belong to the current scene");
    }
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

    view_.begin();
    view_.setProjection(camera.toMatrix());
    view_.setViewMatrix(cameraTransform.inverse().matrix());
    for (const auto entity : current.view<World::Model>()) {
        if (!visible(current, entity)) {
            continue;
        }
        const auto transform = entityTransform(current, entity);
        for (const auto& part : modelResource(current.get<World::Model>(entity).source).parts) {
            const auto& source = materials_.at(part.material);
            Graphics::Material material;
            material.color = Colorf(source.albedo.x(), source.albedo.y(), source.albedo.z(), source.albedo.w());
            material.lit = source.lit;
            if (!source.albedoTexture.empty()) {
                material.texture = textures_.at(source.albedoTexture).get();
            }
            const Eigen::Affine3f model = transform * part.transform;
            // shortcut: Graphics currently uses albedo and lit; add PBR maps when its passes support them.
            view_.addItem(*meshes_.at(part.mesh), material, model, source.alphaMode == Schema::Material::AlphaMode::Blend);
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
            view_.addLight(light);
        }
    };
    addLights.operator()<World::DirectionalLight>();
    addLights.operator()<World::PointLight>();
    addLights.operator()<World::SpotLight>();
    view_.end();
    renderer_.render(view_, target);
}

VOLCANO_FRAMEWORK_END
