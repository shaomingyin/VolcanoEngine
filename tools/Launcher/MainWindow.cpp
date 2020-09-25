//
//
#include <QSurfaceFormat>

#include "MainWindow.hpp"

MainWindow::MainWindow(Volcano::World &w, QWindow *parent):
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
    if (!m_gc.init())
        return;

    m_target.resize(size());
    m_gc.renderer().setTarget(m_target);

    Volcano::Graphics::glFunctions()->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void MainWindow::paintGL(void)
{
    Volcano::Graphics::glFunctions()->glViewport(0, 0, width(), height());
    Volcano::Graphics::glFunctions()->glClear(GL_COLOR_BUFFER_BIT);

    m_world.buildSnapshot(m_snapshot);

    m_gc.renderer().render(m_snapshot);
}

void MainWindow::resizeGL(int w, int h)
{
    m_target.resize(QSize(w, h));
}
