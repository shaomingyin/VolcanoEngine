//
//
#include <Volcano/Editor/SceneEditorViewport.h>

VOLCANO_EDITOR_BEGIN

SceneEditorViewport::SceneEditorViewport(QWidget* parent)
    : QOpenGLWidget(parent) {
    setWindowTitle("World");
}

void SceneEditorViewport::initializeGL() {
    graphics_renderer_ = std::make_unique<Graphics::Renderer>();
}

void SceneEditorViewport::resizeGL(int w, int h) {
    graphics_renderer_ = std::make_unique<Graphics::Renderer>();
}

void SceneEditorViewport::paintGL() {
    if (!graphics_renderer_ || !game_scene_) {
        return;
    }

    // TODO
}

VOLCANO_EDITOR_END
