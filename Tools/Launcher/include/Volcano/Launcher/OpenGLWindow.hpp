//
//
#ifndef VOLCANO_LAUNCHER_OPENGLWINDOW_HPP
#define VOLCANO_LAUNCHER_OPENGLWINDOW_HPP

#include <QOpenGLWindow>

#include <Volcano/Graphics/OpenGL/Renderer.hpp>
#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

class OpenGLWindow: public QOpenGLWindow {
    Q_OBJECT

public:
    OpenGLWindow();
    ~OpenGLWindow() override = default;

protected:
    void initializeGL() override;
    void paintGL() override;
    void paintOverGL() override;
    void paintUnderGL() override;
    void resizeGL(int w, int h) override;

private:
    Graphics::OpenGL::Renderer renderer_;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_OPENGLWINDOW_HPP
