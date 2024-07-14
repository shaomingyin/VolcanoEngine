//
//
#include <Volcano/Launcher/OpenGLWindow.hpp>

VOLCANO_LAUNCHER_BEGIN

OpenGLWindow::OpenGLWindow() {
}

void OpenGLWindow::initializeGL() {
    if (renderer_.isInited()) {
        return;
    }
    if (!renderer_.init(width(), height())) {
        return;
    }
    // TODO
}

void OpenGLWindow::paintGL() {
    QOpenGLWindow::paintGL();
}

void OpenGLWindow::paintOverGL() {
    QOpenGLWindow::paintOverGL();
}

void OpenGLWindow::paintUnderGL() {
    QOpenGLWindow::paintUnderGL();
}

void OpenGLWindow::resizeGL(int w, int h) {
    QOpenGLWindow::resizeGL(w, h);
}

VOLCANO_LAUNCHER_END
