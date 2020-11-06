//
//
#include <QMutexLocker>
#include <QSurfaceFormat>
#include <QCoreApplication>

#include <Volcano/Player/MainWindow.hpp>

VOLCANO_PLAYER_BEGIN

MainWindow::MainWindow(QWindow *parent):
    QOpenGLWindow(NoPartialUpdate, parent),
    m_gameThread(nullptr),
    m_gameWorld(nullptr),
    m_renderer(nullptr),
    m_viewRendering(0)
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
    if (m_gameThread != nullptr)
    {
        {
            QMutexLocker locker(&m_gameMutex);
            m_gameRunning = false;
            m_gameCond.wakeOne();
        }

        m_gameThread->wait();
        delete m_gameThread;
    }

    if (m_renderer != nullptr)
    {
        makeCurrent();
        delete m_renderer;
    }
}

bool MainWindow::init(const QUrl &gameUrl)
{
    Q_ASSERT(gameUrl.isValid());
    Q_ASSERT(m_gameThread == nullptr);
    Q_ASSERT(m_gameWorld == nullptr);

    m_gameUrl = gameUrl;
    m_gameRunning = true;
    m_gameThread = QThread::create(&MainWindow::gameThread, this);
    m_gameThread->start();

    m_frameTimer = startTimer(16, Qt::PreciseTimer);
    m_frameCountTimer = startTimer(1000);
    m_frameCount = 0;
    m_frameCountPerSecond = 0;

    return true;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_frameTimer)
    {
        requestUpdate();
        m_frameCount += 1;
        return;
    }

    if (event->timerId() == m_frameCountTimer)
    {
        m_frameCountPerSecond = m_frameCount;
        m_frameCount = 0;
        return;
    }
}

void MainWindow::initializeGL(void)
{
    if (m_renderer != nullptr)
        delete m_renderer;

    m_renderer = new Graphics::Renderer();
    if (m_renderer == nullptr)
        return;

    if (!m_renderer->init(size()))
    {
        delete m_renderer;
        m_renderer = nullptr;
    }

    m_viewFlip[0].reset();
    m_viewFlip[1].reset();
}

void MainWindow::paintGL(void)
{
    if (Q_LIKELY(m_renderer != nullptr))
    {
        {
            QMutexLocker locker(&m_gameMutex);
            m_gameCond.wakeOne();
            m_viewRendering = !m_viewRendering;
        }

        m_renderer->render(m_viewFlip[m_viewRendering]);
    }
}

void MainWindow::resizeGL(int w, int h)
{
    if (Q_LIKELY(m_renderer != nullptr))
        m_renderer->resize(w, h);
}

void MainWindow::gameThread(void)
{
    QQmlApplicationEngine qmlEngine;

    connect(&qmlEngine, &QQmlApplicationEngine::objectCreated, this, &MainWindow::objectCreated, Qt::DirectConnection);
    connect(&qmlEngine, &QQmlApplicationEngine::quit, this, &MainWindow::quit, Qt::DirectConnection);
    connect(&qmlEngine, &QQmlApplicationEngine::warnings, this, &MainWindow::warnings, Qt::DirectConnection);

    qmlEngine.installExtensions(QJSEngine::AllExtensions);
    qmlEngine.load(m_gameUrl);

    QElapsedTimer elapseTimer;
    elapseTimer.start();

    QMutexLocker locker(&m_gameMutex);

    for (;;)
    {
        m_gameCond.wait(&m_gameMutex);

        if (!m_gameRunning)
            break;

        QCoreApplication::processEvents();

        if (Q_LIKELY(m_gameWorld != nullptr))
        {
            m_gameWorld->update(float(elapseTimer.restart()) / 1000.0f);

            Graphics::View &view = m_viewFlip[!m_viewRendering];
            view.reset();
            m_gameWorld->addToView(view);
        }
    }
}

void MainWindow::objectCreated(QObject *object, const QUrl &url)
{
    Q_ASSERT(m_gameWorld == nullptr);
    Q_UNUSED(url);

    m_gameWorld = qobject_cast<Game::World *>(object);
    if (m_gameWorld == nullptr)
    {
        // qFatal
        m_gameRunning = false;
    }
}

void MainWindow::quit(void)
{
    m_gameWorld = nullptr;
    m_gameRunning = false;
}

void MainWindow::warnings(const QList<QQmlError> &warnings)
{
}

VOLCANO_PLAYER_END
