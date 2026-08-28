//
//
#include <Volcano/World/Light.h>
#include <Volcano/World/DirectionalLight.h>
#include <Volcano/World/PointLight.h>
#include <Volcano/World/SpotLight.h>
#include <Volcano/World/OrthographicCamera.h>
#include <Volcano/World/PerspectiveCamera.h>
#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(World::Scene& scene)
    : scene_(scene) {
	spdlog::info("Initializing graphics renderer...");
    //auto models_view = registry_.view<Transform, Game::Model>();
    //for (auto ent: models_view) {
    //    onModelAdded(ent);
    //}
}

Renderer::~Renderer() {
}

void Renderer::build() noexcept {

}

void Renderer::render() const noexcept {

}

VOLCANO_GRAPHICS_END
