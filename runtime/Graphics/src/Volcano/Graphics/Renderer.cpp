//
//
#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(entt::registry& scene)
    : scene_(scene) {
}

void Renderer::reset() noexcept {
    projection_matrix_.setIdentity();
    view_matrix_.setIdentity();
}

void Renderer::build(entt::entity use_camera) noexcept {
    assert(scene_.valid(use_camera));

    auto camera = scene_.try_get<World::Camera>(use_camera);
    assert(camera != nullptr);
    projection_matrix_ = camera->toMatrix();

    auto transform = scene_.try_get<Transform>(use_camera);
    assert(transform != nullptr);
    view_matrix_ = transform->toMatrix();

    auto maps = scene_.view<Transform, World::Map>();

    auto ambient_lights = scene_.view<Transform, World::Light>();

    auto directional_lights = scene_.view<Transform, World::DirectionalLight>();

    auto point_lights = scene_.view<Transform, World::PointLight>();

    auto spot_lights = scene_.view<Transform, World::SpotLight>();

    auto screens = scene_.view<Transform, World::Screen>();

    auto models = scene_.view<Transform, World::Model>();
}

void Renderer::render() noexcept {
}

VOLCANO_GRAPHICS_END
