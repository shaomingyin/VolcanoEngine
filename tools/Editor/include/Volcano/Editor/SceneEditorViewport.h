//
//
#ifndef VOLCANO_EDITOR_SCENEEDITORVIEWPORT_H
#define VOLCANO_EDITOR_SCENEEDITORVIEWPORT_H

#include <memory>

#include <QtOpenGLWidgets/QOpenGLWidget>

#include <Volcano/Graphics/Renderer.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class SceneEditorViewport: public QOpenGLWidget {
    Q_OBJECT

public:
    SceneEditorViewport(QWidget* parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    std::unique_ptr<Graphics::Renderer> graphics_renderer_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_SCENEEDITORVIEWPORT_H
