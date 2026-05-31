//
//
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Camera.h>
#include <Volcano/World/Lighting.h>
#include <Volcano/World/Model.h>
#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(entt::registry& registry)
    : registry_(registry)
    , current_frame_(0) {
    auto models_view = registry_.view<Transform, World::Model>();
    for (auto ent: models_view) {
        onModelAdded(ent);
    }
    registry_.on_construct<Transform>().connect<&Renderer::onModelAdded>(this);
    registry_.on_construct<World::Model>().connect<&Renderer::onModelAdded>(this);
}

Renderer::~Renderer() {
    registry_.on_construct<Transform>().disconnect<&Renderer::onModelAdded>(this);
    registry_.on_construct<World::Model>().disconnect<&Renderer::onModelAdded>(this);
}

void Renderer::build(entt::entity camera_ent) noexcept {
    frame_.reset();

    if (!registry_.valid(camera_ent)) {
        return;
    }

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
        if (orthographic_camera != nullptr) {
		    build(*camera_transform, *orthographic_camera);
        }
    } else {
		build(*camera_transform, *perspective_camera);
    }
}

void Renderer::build(const Transform& transform, const World::PerspectiveCamera& camera) noexcept {
    frame_[Frame::Affine3f::View] = transform.toAffine().inverse();
    frame_[Frame::Matrix4f::Projection] = camera.toMatrix();
    buildWorld();
}

void Renderer::build(const Transform& transform, const World::OrthographicCamera& camera) noexcept {
    frame_[Frame::Affine3f::View] = transform.toAffine().inverse();
    frame_[Frame::Matrix4f::Projection] = camera.toMatrix();
    buildWorld();
}

void Renderer::draw(const sf::RenderTarget& target) const noexcept {
    auto& view = target.getView();
    auto center = view.getCenter();
    auto size = view.getSize();
    frame_.commit(center.x + size.x / 2.0f, center.y + size.y / 2.0f, size.x, size.y);
}

void Renderer::buildWorld() noexcept {
    auto lights = registry_.view<World::Light>();
    for (auto&& [ent, light] : lights.each()) {
        if (registry_.get<World::Inherent>(ent).isEnabled()) {
            frame_.add(light);
        }
    }

    auto directional_lights = registry_.view<World::DirectionalLight>();
    for (auto&& [ent, directional_light] : directional_lights.each()) {
        if (registry_.get<World::Inherent>(ent).isEnabled()) {
            frame_.add(directional_light);
        }
    }

    auto point_lights = registry_.view<Transform, World::PointLight>();
    for (auto&& [ent, transform, point_light] : point_lights.each()) {
        if (registry_.get<World::Inherent>(ent).isEnabled()) {
            frame_.add(transform, point_light);
        }
    }

    auto spot_lights = registry_.view<Transform, World::SpotLight>();
    for (auto&& [ent, transform, spot_light] : spot_lights.each()) {
        if (registry_.get<World::Inherent>(ent).isEnabled()) {
            frame_.add(transform, spot_light);
        }
    }
}

void Renderer::onModelAdded(entt::entity ent) noexcept {

}

void Renderer::onModelRemoved(entt::entity ent) noexcept {
}

VOLCANO_GRAPHICS_END
