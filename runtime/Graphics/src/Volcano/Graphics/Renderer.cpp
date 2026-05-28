//
//
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Camera.h>
#include <Volcano/World/Lighting.h>
#include <Volcano/World/Geometry.h>
#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(const entt::registry& registry)
    : registry_(registry) {
}

void Renderer::reset() noexcept {
}

void Renderer::build(entt::entity camera_ent) noexcept {
    VOLCANO_ASSERT(registry_.valid(camera_ent));

    auto& camera_inherent = registry_.get<World::Inherent>(camera_ent);
    if (!camera_inherent.isEnabled()) {
        return;
    }

    auto camera_transform = registry_.try_get<Transform>(camera_ent);
    if (camera_transform == nullptr) {
        return;
    }

    auto perspective_camera = registry_.try_get<World::PerspectiveCamera>(camera_ent);
    if (perspective_camera == nullptr) {
        auto orthographic_camera = registry_.try_get<World::OrthographicCamera>(camera_ent);
        if (orthographic_camera == nullptr) {
            return;
        }
    } else {

    }

    auto lights = registry_.view<World::Light>();
    for (auto&& [ent, light]: lights.each()) {
        if (!registry_.get<World::Inherent>(ent).isEnabled()) {
            continue;
        }
    }

    auto directional_lights = registry_.view<World::DirectionalLight>();
    for (auto&& [ent, directional_light]: directional_lights.each()) {
        if (!registry_.get<World::Inherent>(ent).isEnabled()) {
            continue;
        }
    }

    auto point_lights = registry_.view<Transform, World::PointLight>();
    for (auto&& [ent, transform, point_light]: point_lights.each()) {
        if (!registry_.get<World::Inherent>(ent).isEnabled()) {
            continue;
        }
    }

    auto spot_lights = registry_.view<Transform, World::SpotLight>();
    for (auto&& [ent, transform, spot_light]: spot_lights.each()) {
    }

    auto boxies = registry_.view<Transform, World::Box>();
    for (auto&& [ent, transform, box]: boxies.each()) {

    }

    auto spheres = registry_.view<Transform, World::Sphere>();
    for (auto&& [ent, transform, sphere] : spheres.each()) {

    }

    auto capsules = registry_.view<Transform, World::Capsule>();
    for (auto&& [ent, transform, capsule] : capsules.each()) {

    }

    auto cylinders = registry_.view<Transform, World::Cylinder>();
    for (auto&& [ent, transform, cylinder] : cylinders.each()) {

    }

    auto planes = registry_.view<Transform, World::Plane>();
    for (auto&& [ent, transform, plane] : planes.each()) {

    }
}

void Renderer::render(const sf::RenderTarget& target) const noexcept {
    
}

VOLCANO_GRAPHICS_END
