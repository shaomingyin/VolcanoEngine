//
//
#include <Volcano/Error.h>
#include <Volcano/ScopeGuard.h>
#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer() {
}

Renderer::~Renderer() {
}

void Renderer::render(const View& view) {

	vector_graphics_pipeline_.apply();
	//view.hud().render();
}

VOLCANO_GRAPHICS_END
