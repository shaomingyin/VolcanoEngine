//
//
#include <Volcano/Editor/WorldEditorViewport.h>

VOLCANO_EDITOR_BEGIN

WorldEditorViewport::WorldEditorViewport(QWidget* parent)
    : QOpenGLWidget(parent) {
    setWindowTitle("World");
}

void WorldEditorViewport::initializeGL() {
    graphics_renderer_ = std::make_unique<Graphics::Renderer>();
}

void WorldEditorViewport::resizeGL(int w, int h) {
    graphics_renderer_ = std::make_unique<Graphics::Renderer>();
}

void WorldEditorViewport::paintGL() {
    if (!graphics_renderer_) {
        return;
    }

    // TODO
}

VOLCANO_EDITOR_END
