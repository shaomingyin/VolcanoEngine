//
//
#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(Stage::Scene& scene)
    : scene_(scene) {
}

void Renderer::reset() noexcept {
    projection_matrix_.setIdentity();
    view_matrix_.setIdentity();
}

void Renderer::build(entt::entity camera) noexcept {
    assert(scene_.valid(camera));

    auto perspective_camera = scene_.try_get<Stage::PerspectiveCamera>(camera);
	auto orthographic_camera = scene_.try_get<Stage::OrthographicCamera>(camera);
    if (perspective_camera != nullptr) {
		projection_matrix_ = perspective_camera->toMatrix();
    } else if (orthographic_camera != nullptr) {
		projection_matrix_ = orthographic_camera->toMatrix();
    } else {
        assert(false && "Camera component not found");
	}

    auto transform = scene_.try_get<Transform>(camera);
    if (transform != nullptr) {
        view_matrix_ = transform->toMatrix();
    }

    auto maps = scene_.view<Transform, Stage::Map>();

    auto ambient_lights = scene_.view<Transform, Stage::Light>();

    auto directional_lights = scene_.view<Transform, Stage::DirectionalLight>();

    auto point_lights = scene_.view<Transform, Stage::PointLight>();

    auto spot_lights = scene_.view<Transform, Stage::SpotLight>();

    auto screens = scene_.view<Transform, Stage::Screen>();

    //auto models = scene_.view<Transform, Stage::Model>();
}

void Renderer::render() noexcept {
}

VOLCANO_GRAPHICS_END
