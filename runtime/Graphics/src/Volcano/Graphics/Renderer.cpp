//
//
#include <Volcano/Error.h>
#include <Volcano/ScopeGuard.h>
#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(entt::registry& registry)
	: registry_(registry) {
}

Renderer::~Renderer() {
}

void Renderer::frame(Duration elapsed) {
}

void Renderer::buildView(View& view) const {
}

VOLCANO_GRAPHICS_END
