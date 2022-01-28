//
//
#include <memory>

#include <Volcano/Graphics/QuickSurface.hpp>

VOLCANO_GRAPHICS_BEGIN

QuickSurface::QuickSurface(QObject *parent):
    QObject(parent),
    m_renderControl(nullptr),
    m_renderTarget(nullptr)
{
}

QuickSurface::~QuickSurface(void)
{
}

bool QuickSurface::init(QQuickWindow *window)
{
    Q_ASSERT(window != nullptr);
    Q_ASSERT(m_renderControl == nullptr);
    Q_ASSERT(m_renderTarget == nullptr);

    auto renderControl = std::make_unique<QQuickRenderControl>(this);
    if (!renderControl)
        return false;

    auto renderTarget = std::make_unique<QQuickRenderTarget>(*createRenderTarget(window->size(), 0));
    if (!renderTarget)
        return false;

    std::unique_ptr<QQuickGraphicsDevice> graphicsDevice(createGraphicsDevice());
    if (!graphicsDevice)
        return false;

    window->setGraphicsDevice(*graphicsDevice);
    if (!renderControl->initialize())
        return false;

    return true;
}

void QuickSurface::update(void)
{
    Q_ASSERT(m_renderControl != nullptr);

    m_renderControl->polishItems();
    m_renderControl->beginFrame();
    m_renderControl->sync();
}

void QuickSurface::render(void)
{
    Q_ASSERT(m_renderControl != nullptr);

    m_renderControl->render();
    m_renderControl->endFrame();
}

VOLCANO_GRAPHICS_END
