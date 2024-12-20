//
//
#include <QScopeGuard>

#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(World::Scene& scene, QObject* parent)
    : QObject(parent)
    , scene_(scene) {
}

Renderer::~Renderer() {
}

void Renderer::update(Duration elapsed) {
}

void Renderer::render() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

VOLCANO_GRAPHICS_END
