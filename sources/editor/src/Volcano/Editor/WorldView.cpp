//
//
#include <QWindow>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Editor/WorldView.hpp>

VOLCANO_EDITOR_BEGIN

WorldView::WorldView(QWidget *parent):
    QOpenGLWidget(parent),
    m_graphicsMemory(nullptr),
    m_graphicsRenderer(nullptr)
{
    QSurfaceFormat format;

    format.setVersion(4, 5);
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    format.setAlphaBufferSize(16);
    format.setDepthBufferSize(24);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    
    setFormat(format);
}

WorldView::~WorldView(void)
{
}

void WorldView::initializeGL(void)
{
    if (m_graphicsRenderer != nullptr)
        return;

    auto graphicsRenderer = std::make_shared<UI::Graphics::Renderer>();
    if (!graphicsRenderer || !m_graphicsRenderer->init(width(), height()))
        return;

    auto graphicsMemory = std::make_shared<UI::Graphics::Memory>();
    if (!graphicsMemory)
        return;

    m_graphicsRenderer = graphicsRenderer;
    m_graphicsMemory = graphicsMemory;
}

void WorldView::paintGL(void)
{
    if (m_graphicsRenderer) {
        m_graphicsRenderer->beginFrame();
        // TODO
        m_graphicsRenderer->endFrame();
    }
}

void WorldView::resizeGL(int w, int h)
{
}

VOLCANO_EDITOR_END
