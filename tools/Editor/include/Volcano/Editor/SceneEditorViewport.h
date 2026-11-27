//
//
#ifndef VOLCANO_EDITOR_SCENEEDITORVIEWPORT_H
#define VOLCANO_EDITOR_SCENEEDITORVIEWPORT_H

#include <memory>

#include <QOpenGLWidget>
#include <QPointer>

#include <Volcano/World/Scene.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class SceneEditorViewport: public QOpenGLWidget {
    Q_OBJECT

public:
    SceneEditorViewport(QWidget* parent = nullptr);
    ~SceneEditorViewport() override;

    void setScene(World::Scene* scene);
    void setCamera(World::Camera* camera);

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    void releaseRenderer();

    std::unique_ptr<Graphics::Renderer> renderer_;
    World::Scene* scene_ = nullptr;
    QPointer<World::Camera> camera_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_SCENEEDITORVIEWPORT_H
