//
//
#include <QEventLoop>
#include <QSurfaceFormat>
#include <QQmlEngine>
#include <QQmlComponent>

#include <Volcano/Game/World.hpp>
#include <Volcano/Player/MainWindow.hpp>

VOLCANO_PLAYER_BEGIN

MainWindow::MainWindow(const QUrl &gameUrl, QWindow *parent):
    QOpenGLWindow(NoPartialUpdate, parent),
    m_gameUrl(gameUrl),
    m_gameThread(nullptr)
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
    if (m_gameThread->isRunning())
    {
        // TODO
    }

    if (m_renderer.isInit())
    {
        makeCurrent();
        m_renderer.shutdown();
    }
}

void MainWindow::initializeGL(void)
{
    if (!m_gameUrl.isValid())
        return;

    if (!m_renderer.init(size()))
        return;

    m_gameThread = QThread::create(&MainWindow::gameThread, this);
    if (m_gameThread == nullptr)
        m_renderer.shutdown();
}

void MainWindow::paintGL(void)
{
    if (Q_UNLIKELY(!m_renderer.isInit()))
        return;

    Graphics::View v;
    m_renderer.render(v);
}

void MainWindow::resizeGL(int w, int h)
{
    if (Q_UNLIKELY(!m_renderer.isInit()))
        return;

    m_renderer.resize(w, h);
}

void MainWindow::gameThread(void)
{
    Game::VM game(m_gameUrl);

    if (game.start())
    {
        game.exec();
        game.stop();
    }
}

VOLCANO_PLAYER_END
