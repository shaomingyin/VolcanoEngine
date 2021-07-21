//
//
#include <QQuickOpenGLUtils>

#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Graphics/Renderer.hpp>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(void):
    m_view(nullptr)
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
    if (Q_LIKELY(m_view != nullptr)) {
        m_view->render();
        QQuickOpenGLUtils::resetOpenGLState();
    }
}

void Renderer::synchronize(QQuickFramebufferObject *item)
{
#ifdef VOLCANO_DEBUG
    auto camera = qobject_cast<Camera *>(item);
#else
    auto camera = static_cast<Camera *>(item);
#endif

    Q_ASSERT(camera != nullptr);

    m_view = &camera->view();
    if (m_view != nullptr)
        m_view->beforeRender();
}

VOLCANO_GRAPHICS_END
