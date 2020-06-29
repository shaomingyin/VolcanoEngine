//
//
#include <Volcano/Launcher.h>

VOLCANO_BEGIN

Launcher::Launcher(void):
    m_view(nullptr),
    m_viewWidget(nullptr),
    m_renderer(nullptr)
{
}

Launcher::~Launcher(void)
{
    if (m_viewWidget != nullptr)
        delete m_viewWidget;

    // 'm_view' is owned by m_viewWidget,
    // so 'delete m_view' is not needed.
}

bool Launcher::init(const QUrl &url)
{
    Q_ASSERT(m_view == nullptr);
    Q_ASSERT(m_viewWidget == nullptr);

    m_view = new QQuickView();
    if (m_view == nullptr)
        return false;

    m_view->setPersistentOpenGLContext(true);
    m_view->setSource(url);

    m_viewWidget = QWidget::createWindowContainer(m_view, this);
    if (m_viewWidget == nullptr) {
        delete m_view;
        m_view = nullptr;
        return false;
    }

    connect(m_view, &QQuickView::sceneGraphInitialized,
        this, &Launcher::initRendererContext, Qt::DirectConnection);

    connect(m_view, &QQuickView::sceneGraphInvalidated,
        this, &Launcher::releaseRendererContext, Qt::DirectConnection);

    connect(m_view, &QQuickView::beforeRendering,
            this, &Launcher::beforeRendering, Qt::DirectConnection);

    setCentralWidget(m_viewWidget);
    setWindowTitle("Volcano");

    return true;
}

void Launcher::initRendererContext(void)
{
    if (m_renderer != nullptr)
        return;

    m_renderer = new Renderer::Context();
    if (m_renderer == nullptr)
        return;

    if (!m_renderer->init())
    {
        delete m_renderer;
        m_renderer= nullptr;
    }

    m_renderer->makeCurrent();
}

void Launcher::releaseRendererContext(void)
{
    if (m_renderer != nullptr)
    {
        delete m_renderer;
        m_renderer = nullptr;
    }
}

void Launcher::beforeRendering(void)
{
    if (m_renderer != nullptr)
        m_renderer->makeCurrent();
}

VOLCANO_END
