//
//
#ifndef VOLCANO_EDITOR_WORLDVIEW_HPP
#define VOLCANO_EDITOR_WORLDVIEW_HPP

#include <QOpenGLWidget>

#include <Volcano/Game/WorldBase.hpp>
#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Graphics/World.hpp>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class WorldView: public QOpenGLWidget {
    Q_OBJECT

public:
    WorldView(QWidget *parent = nullptr);
    ~WorldView(void) override;

public:
    //void setGameWorld()

protected:
    void initializeGL(void) override;
    void paintGL(void) override;
    void resizeGL(int w, int h) override;

private:
    Graphics::Camera m_graphicsCamera;
    Graphics::Renderer m_graphicsRenderer;
    Graphics::World m_graphicsWorld;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_WORLDVIEW_HPP
