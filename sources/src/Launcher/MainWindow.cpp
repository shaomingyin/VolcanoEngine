//
//
#include <QEventLoop>
#include <QSurfaceFormat>
#include <QQmlEngine>
#include <QQmlComponent>

#include <Volcano/Game/World.hpp>
#include <Volcano/Launcher/MainWindow.hpp>

VOLCANO_LAUNCHER_BEGIN

MainWindow::MainWindow(QWindow *parent):
    QOpenGLWindow(NoPartialUpdate, parent),
    m_thread(nullptr)
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

bool MainWindow::init(const QUrl &url)
{
    Q_ASSERT(url.isValid());
    Q_ASSERT(m_thread == nullptr);

    m_url = url;

    m_thread = QThread::create(&MainWindow::run, this);
    if (m_thread == nullptr)
        return false;

    return true;
}

void MainWindow::initializeGL(void)
{
    m_renderer.init(size());
}

void MainWindow::paintGL(void)
{
    if (Q_UNLIKELY(!m_renderer.isInit()))
        return;
}

void MainWindow::resizeGL(int w, int h)
{
    m_renderer.resize(w, h);
}

void MainWindow::run(void)
{
}

VOLCANO_LAUNCHER_END
