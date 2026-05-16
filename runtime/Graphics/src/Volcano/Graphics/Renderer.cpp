//
//
#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer() {
}

void Renderer::reset() noexcept {
}

void Renderer::build(const World::Scene& scene, entt::entity camera) noexcept {
    assert(scene.valid(camera));
}

void Renderer::render(Target& target) noexcept {
    // draw(view_[0], target);
}

VOLCANO_GRAPHICS_END
