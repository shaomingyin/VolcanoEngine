//
//
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Camera.h>
#include <Volcano/World/Lighting.h>
#include <Volcano/World/Model.h>
#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(World::Scene& scene)
    : scene_(scene)
    , current_frame_(0) {
    auto models_view = scene_.view<Transform, World::Model>();
    for (auto ent: models_view) {
        onModelAdded(ent);
    }
    scene_.on_construct<Transform>().connect<&Renderer::onModelAdded>(this);
    scene_.on_construct<World::Model>().connect<&Renderer::onModelAdded>(this);
}

Renderer::~Renderer() {
    scene_.on_construct<Transform>().disconnect<&Renderer::onModelAdded>(this);
    scene_.on_construct<World::Model>().disconnect<&Renderer::onModelAdded>(this);
}

void Renderer::build(entt::entity camera_ent) noexcept {
    frame_.reset();

    buildCamera(camera_ent);

    switch (scene_.state()) {
    case World::Scene::State::Playing:
        buildPlaying();
        break;
    case World::Scene::State::Loading:
        buildLoading();
        break;
    case World::Scene::State::Error:
        buildError();
        break;
    }

}

void Renderer::draw(const sf::RenderTarget& target) const noexcept {
    auto& view = target.getView();
    auto center = view.getCenter();
    auto size = view.getSize();
    frame_.commit(center.x + size.x / 2.0f, center.y + size.y / 2.0f, size.x, size.y);
}

void Renderer::buildCamera(entt::entity ent) noexcept {
    if (!scene_.valid(ent)) {
        return;
    }

    auto& inherent = scene_.get<World::Inherent>(ent);
    if (!inherent.isEnabled()) {
        return;
    }

    auto transform = scene_.try_get<Transform>(ent);
    if (transform == nullptr) {
        return;
    }

    frame_[Frame::Affine3f::View] = transform->toMatrix();

    auto perspective = scene_.try_get<World::PerspectiveCamera>(ent);
    if (perspective == nullptr) {
        auto orthographic = scene_.try_get<World::OrthographicCamera>(ent);
        if (orthographic == nullptr) {
            return;
        }
        frame_[Frame::Matrix4f::Projection] = orthographic->toMatrix();
    } else {
        frame_[Frame::Matrix4f::Projection] = perspective->toMatrix();
    }
}

void Renderer::buildLoading() noexcept {

}

void Renderer::buildPlaying() noexcept {
    auto lights = scene_.view<World::Light>();
    for (auto&& [ent, light] : lights.each()) {
        if (scene_.get<World::Inherent>(ent).isEnabled()) {
            frame_.add(light);
        }
    }

    auto directional_lights = scene_.view<World::DirectionalLight>();
    for (auto&& [ent, directional_light] : directional_lights.each()) {
        if (scene_.get<World::Inherent>(ent).isEnabled()) {
            frame_.add(directional_light);
        }
    }

    auto point_lights = scene_.view<Transform, World::PointLight>();
    for (auto&& [ent, transform, point_light] : point_lights.each()) {
        if (scene_.get<World::Inherent>(ent).isEnabled()) {
            frame_.add(transform, point_light);
        }
    }

    auto spot_lights = scene_.view<Transform, World::SpotLight>();
    for (auto&& [ent, transform, spot_light] : spot_lights.each()) {
        if (scene_.get<World::Inherent>(ent).isEnabled()) {
            frame_.add(transform, spot_light);
        }
    }
}

void Renderer::buildError() noexcept {

}

void Renderer::onModelAdded(entt::entity ent) noexcept {

}

void Renderer::onModelRemoved(entt::entity ent) noexcept {
}

VOLCANO_GRAPHICS_END
