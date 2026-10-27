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
    ~SceneEditorViewport() override;

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    void releaseGraphics();

private:
    std::unique_ptr<Graphics::Renderer> graphics_renderer_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_SCENEEDITORVIEWPORT_H
