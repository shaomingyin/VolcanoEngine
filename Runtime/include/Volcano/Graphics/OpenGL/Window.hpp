//
//
#ifndef VOLCANO_GRAPHICS_OPENGL_WINDOW_HPP
#define VOLCANO_GRAPHICS_OPENGL_WINDOW_HPP

#include <QOpenGLWindow>

#include <Volcano/Graphics/OpenGL/Common.hpp>
#include <Volcano/Graphics/OpenGL/Renderer.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

class Window: public QOpenGLWindow {
    Q_OBJECT

public:
    Window(void);
    ~Window(void) override;

protected:
    void initializeGL(void) override;
    void paintGL(void) override;
    void resizeGL(int w, int h) override;

private:
    Renderer m_renderer;
};

VOLCANO_GRAPHICS_OPENGL_END

#endif // VOLCANO_GRAPHICS_OPENGL_WINDOW_HPP
