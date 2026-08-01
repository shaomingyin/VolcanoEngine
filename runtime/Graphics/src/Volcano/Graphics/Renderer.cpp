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
