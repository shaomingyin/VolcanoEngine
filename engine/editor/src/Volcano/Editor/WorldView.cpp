//
//
#include <QWindow>
#include <QScopeGuard>

#include <Volcano/Editor/WorldView.hpp>

VOLCANO_EDITOR_BEGIN

WorldView::WorldView(QWidget *parent):
    QOpenGLWidget(parent),
    m_graphicsMemory(nullptr),
    m_graphicsRenderer(nullptr)
{
    QSurfaceFormat format;
    UI::Graphics::makeSurfaceFormat(format);
    setFormat(format);
}

WorldView::~WorldView(void)
{
}

void WorldView::initializeGL(void)
{
    if (m_graphicsRenderer != nullptr)
        return;

    m_graphicsRenderer = new UI::Graphics::Renderer(this);
    if (m_graphicsRenderer == nullptr)
        return;

    auto graphicsRendererGuard = qScopeGuard([&]() {
        delete m_graphicsRenderer;
        m_graphicsRenderer = nullptr;
    });

    if (!m_graphicsRenderer->init(context()->surface()))
        return;

    if (m_graphicsMemory != nullptr)
        return;

    m_graphicsMemory = new UI::Graphics::Memory(this);
    if (m_graphicsMemory == nullptr)
        return;

    auto graphicsMemoryGuard = qScopeGuard([&]() {
        delete m_graphicsMemory;
        m_graphicsMemory = nullptr;
    });

    graphicsRendererGuard.dismiss();
    graphicsMemoryGuard.dismiss();
}

void WorldView::paintGL(void)
{
    if (m_graphicsRenderer != nullptr) {
        m_graphicsRenderer->beginFrame();
        m_graphicsRenderer->endFrame();
    }
}

void WorldView::resizeGL(int w, int h)
{
}

VOLCANO_EDITOR_END
