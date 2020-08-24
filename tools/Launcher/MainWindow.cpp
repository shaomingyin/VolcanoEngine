//
//
#include <QSize>
#include <QScopedPointer>
#include <QSurfaceFormat>

#include "MainWindow.hpp"

MainWindow::MainWindow(QScreen *screen):
    QWindow(screen),
    m_glContext(nullptr),
    m_qmlComponent(nullptr),
    m_renderer(nullptr),
    m_world(nullptr),
    m_frameTimer(0)
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
    if (m_renderer != nullptr)
        delete m_renderer;
}

bool MainWindow::init(void)
{
    Q_ASSERT(m_glContext == nullptr);
    Q_ASSERT(m_renderer == nullptr);

    QWindow::create();

    QScopedPointer<QOpenGLContext> glContext(new QOpenGLContext(this));
    if (!glContext)
        return false;

    glContext->setFormat(format());
    if (!glContext->create())
        return false;

    if (!glContext->makeCurrent(this))
        return false;

    QScopedPointer<Volcano::OpenGL::Renderer> renderer(new Volcano::OpenGL::Renderer());
    if (!renderer->init())
        return false;

    m_glContext = glContext.take();
    m_renderer = renderer.take();

    m_view.setClearEnabled(true);
    m_view.setClearColor(QColor::fromRgb(255, 255, 255));

    m_target.resize(width(), height());

    return true;
}

bool MainWindow::load(const QUrl &url)
{
    if (m_frameTimer > 0)
    {
        killTimer(m_frameTimer);
        m_frameTimer = 0;
    }

    if (m_world != nullptr)
    {
        delete m_world;
        m_world = nullptr;
    }

    if (m_qmlComponent != nullptr)
    {
        delete m_qmlComponent;
        m_qmlComponent = nullptr;
    }

    qDebug() << "Loading game...";

    m_qmlComponent = new QQmlComponent(&m_qmlEngine, url, this);
    if (m_qmlComponent->isLoading())
        connect(m_qmlComponent, &QQmlComponent::statusChanged, this, &MainWindow::startup);
    else
        startup();

    return true;
}

void MainWindow::timerEvent(QTimerEvent *evt)
{
    if (Q_LIKELY(evt->timerId() == m_frameTimer))
    {
        qint64 elapsed = m_elapsedTimer.restart();
        frame(float(elapsed));
    }
}

void MainWindow::startup(void)
{
    Q_ASSERT(m_qmlComponent != nullptr);

    disconnect(m_qmlComponent, &QQmlComponent::statusChanged, this, &MainWindow::startup);

    if (m_qmlComponent->isError())
    {
        const QList<QQmlError> errorList = m_qmlComponent->errors();
        for (const QQmlError &error : errorList)
            qWarning() << error.url() << error.line() << error;
        return;
    }

    QObject *rootObject = m_qmlComponent->create();
    if (m_qmlComponent->isError() || rootObject == nullptr)
    {
        const QList<QQmlError> errorList = m_qmlComponent->errors();
        for (const QQmlError &error : errorList)
            qWarning() << error.url() << error.line() << error;
        return;
    }

    Q_ASSERT(m_world == nullptr);
    m_world = qobject_cast<Volcano::World *>(rootObject);
    if (m_world == nullptr)
    {
        qWarning("Root object must be a 'Volcano.World'.");
        delete rootObject;
        return;
    }

    m_world->setParent(this);
    m_world->setWindow(this);

    Q_ASSERT(m_frameTimer == 0);
    m_frameTimer = startTimer(16, Qt::PreciseTimer);
    if (m_frameTimer > 0)
        m_elapsedTimer.start();
}

void MainWindow::frame(float elapsed)
{
    Q_ASSERT(m_world != nullptr);
    m_world->update(elapsed, true);

    Q_ASSERT(m_glContext != nullptr);
    if (Q_LIKELY(m_glContext->makeCurrent(this)))
    {
        QSize sz = size();
        if (Q_UNLIKELY(sz != m_target.size()))
            m_target.resize(sz);

        m_view.reset();

        m_world->draw(m_view, true);

        Q_ASSERT(m_renderer != nullptr);
        m_renderer->render(m_view, m_target);

        m_glContext->swapBuffers(this);
    }
}
