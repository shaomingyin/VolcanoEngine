//
//
#ifndef VOLCANO_EDITOR_VIEWPORT_HPP
#define VOLCANO_EDITOR_VIEWPORT_HPP

#include <QOpenGLWidget>

#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Graphics/View.hpp>
#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class Viewport: public QOpenGLWidget {
    Q_OBJECT

public:
    Viewport(QWidget *parent = nullptr);
    ~Viewport(void) override;

public:
    Game::World *gameWorld(void);
    void setGameWorld(Game::World *gameWorld);

signals:
    void gameWorldChanged(Game::World *gameWorld);

protected:
    void initializeGL(void) override;
    void paintGL(void) override;
    void resizeGL(int w, int h) override;

private:
    Game::World *m_gameWorld;
    Graphics::OpenGLFunctions *m_glFunctions;
    Graphics::Camera m_graphicsCamera;
    Graphics::View *m_graphicsView;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_VIEWPORT_HPP
