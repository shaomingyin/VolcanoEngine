//
//
#include <QSurfaceFormat>

#include "MainWindow.hpp"

MainWindow::MainWindow(Volcano::Game::World &w, QWindow *parent):
    QOpenGLWindow(NoPartialUpdate, parent),
    m_world(w)
{
    QSurfaceFormat format;

    format.setVersion(3, 3);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    format.setAlphaBufferSize(0);
    format.setDepthBufferSize(16);

#ifdef VOLCANO_DEBUG
    format.setOption(QSurfaceFormat::DebugContext, true);
#endif

    setFormat(format);
}

MainWindow::~MainWindow(void)
{
}

void MainWindow::initializeGL(void)
{
}

void MainWindow::paintGL(void)
{
}

void MainWindow::resizeGL(int w, int h)
{
}
