//
//
#include <QSize>
#include <QScopedPointer>
#include <QSurfaceFormat>

#include "MainWindow.hpp"

MainWindow::MainWindow(void)
{
    QSurfaceFormat format;

    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    format.setAlphaBufferSize(8);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);

#ifdef VOLCANO_DEBUG
    format.setOption(QSurfaceFormat::DebugContext, 1);
#endif

    setFormat(format);
    setSurfaceType(OpenGLSurface);
}

MainWindow::~MainWindow(void)
{
}

void MainWindow::initializeGL(void)
{
    auto gc = Volcano::Drivers::Graphics::defaultContext();
    if (gc == nullptr)
        return;

    m_renderer = &gc->renderer();
}

void MainWindow::paintGL(void)
{
    // m_renderer->
}

void MainWindow::paintOverGL(void)
{
}

void MainWindow::paintUnderGL(void)
{
}

void MainWindow::resizeGL(int w, int h)
{
    m_target.resize(w, h);
}
