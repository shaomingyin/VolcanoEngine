//
//
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
    if (!camera_inherent.enabled()) {
        return;
    }

    auto camera_transform = registry_.try_get<Transform>(camera_ent);
    if (camera_transform == nullptr) {
        return;
    }

    auto camera = registry_.try_get<World::Camera>(camera_ent);
    if (camera != nullptr) {
		build(*camera_transform, *camera);
    }
}

void Renderer::build(const Transform& transform, const World::Camera& camera) noexcept {
    frame_[Frame::Affine3f::View] = transform.toAffine().inverse();
    //frame_[Frame::Matrix4f::Projection] = camera.toMatrix();
    auto lights = registry_.view<World::Light>();
    for (auto&& [ent, light] : lights.each()) {
        if (registry_.get<World::Inherent>(ent).enabled()) {
            frame_.add(light);
        }
    }
}

void Renderer::draw(const sf::RenderTarget& target) const noexcept {
    auto& view = target.getView();
    auto center = view.getCenter();
    auto size = view.getSize();
    frame_.commit(center.x + size.x / 2.0f, center.y + size.y / 2.0f, size.x, size.y);
}

void Renderer::onModelAdded(entt::entity ent) noexcept {

}

void Renderer::onModelRemoved(entt::entity ent) noexcept {
}

VOLCANO_GRAPHICS_END
