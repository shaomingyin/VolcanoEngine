//
//
#include <QQuickOpenGLUtils>

#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Graphics/Renderer.hpp>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(Camera &camera):
    m_camera(camera),
    m_vs(nullptr)
{
}

Renderer::~Renderer(void)
{
}

bool Renderer::init(void)
{
    // TODO init resources...

    return true;
}

void Renderer::render(void)
{
    if (Q_UNLIKELY(m_vs == nullptr))
        return;

    // TODO render 'm_vs'...

    m_camera.unlockVisibleSet();

    QQuickOpenGLUtils::resetOpenGLState();
}

void Renderer::synchronize(QQuickFramebufferObject *item)
{
    Q_ASSERT(static_cast<QQuickFramebufferObject *>(&m_camera) == item);

    m_vs = m_camera.lockVisibleSet();
}

VOLCANO_GRAPHICS_END
