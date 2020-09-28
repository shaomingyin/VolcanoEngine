//
//
#include <QEventLoop>
#include <QSurfaceFormat>
#include <QQmlEngine>
#include <QQmlComponent>

#include <Volcano/Game/World.hpp>

#include "MainWindow.hpp"

MainWindow::MainWindow(QWindow *parent):
    QOpenGLWindow(NoPartialUpdate, parent),
    m_vm(nullptr),
    m_thread(nullptr),
    m_renderer(nullptr),
    m_target(nullptr)
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
    Q_ASSERT(m_renderer == nullptr);
    Q_ASSERT(m_target == nullptr);

    m_renderer = new Volcano::Graphics::OpenGL::Renderer();
    if (m_renderer == nullptr)
        return;

    if (!m_renderer->init())
    {
        delete m_renderer;
        m_renderer = nullptr;
    }

    m_target = new Volcano::Graphics::OpenGL::Target(width(), height());
    if (m_target == nullptr)
    {
        delete m_renderer;
        m_renderer = nullptr;
    }

    m_renderer->setTarget(m_target);
}

void MainWindow::paintGL(void)
{
    if (m_renderer == nullptr)
        return;
}

void MainWindow::resizeGL(int w, int h)
{
    if (m_target != nullptr)
        m_target->resize(w, h);
}

void MainWindow::run(void)
{
    m_vm = new Volcano::Game::VM(m_url);
    if (m_vm == nullptr)
        return;

    if (m_vm->start())
        m_vm->exec();

    delete m_vm;
    m_vm = nullptr;
}
