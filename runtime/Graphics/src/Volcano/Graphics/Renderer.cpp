//
//
#include <Volcano/Graphics/Light.h>
#include <Volcano/Graphics/DirectionalLight.h>
#include <Volcano/Graphics/PointLight.h>
#include <Volcano/Graphics/SpotLight.h>
#include <Volcano/Graphics/OrthographicCamera.h>
#include <Volcano/Graphics/PerspectiveCamera.h>
#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(const entt::registry& registry)
    : registry_(registry)
    , current_frame_(0) {
	spdlog::info("Initializing graphics renderer...");
    //auto models_view = registry_.view<Transform, Game::Model>();
    //for (auto ent: models_view) {
    //    onModelAdded(ent);
    //}
}

Renderer::~Renderer() {
}

void Renderer::build(entt::entity camera_ent) noexcept {
    frame_.reset();

    if (!registry_.valid(camera_ent)) {
        return;
    }

    auto camera_transform = registry_.try_get<Transform>(camera_ent);
    if (camera_transform == nullptr) {
        return;
    }

    auto perspective_camera = registry_.try_get<PerspectiveCamera>(camera_ent);
    if (perspective_camera == nullptr) {
		auto orthographic_camera = registry_.try_get<OrthographicCamera>(camera_ent);
        if (orthographic_camera == nullptr) {
            return;
        }
		build(*camera_transform, *orthographic_camera);
    } else {
		build(*camera_transform, *perspective_camera);
    }
}

void Renderer::build(const Transform& transform, const Camera& camera) noexcept {
    frame_[Frame::Bool::Clear] = true;
    frame_[Frame::Color::Clear] = sf::Color::Magenta;
    frame_[Frame::Affine3f::View] = transform.toAffine().inverse();
    frame_[Frame::Matrix4f::Projection] = camera.toMatrix();
    auto lights = registry_.view<Light>();
    for (auto&& [ent, light] : lights.each()) {
        frame_.add(light);
    }
}

void Renderer::draw(const sf::RenderTarget& target) const noexcept {
    auto& view = target.getView();
    auto viewport = target.getViewport(view);
    frame_.commit(viewport.position.x, viewport.position.y, viewport.size.x, viewport.size.y);
}

void Renderer::onModelAdded(entt::entity ent) noexcept {

}

void Renderer::onModelRemoved(entt::entity ent) noexcept {
}

VOLCANO_GRAPHICS_END
