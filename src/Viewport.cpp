//
//
#include <QQuickWindow>

#include <Volcano/Camera.hpp>
#include <Volcano/DirectionalLight.hpp>
#include <Volcano/Entity.hpp>
#include <Volcano/Model.hpp>
#include <Volcano/OpenGL.hpp>
#include <Volcano/PointLight.hpp>
#include <Volcano/Scene.hpp>
#include <Volcano/SpotLight.hpp>
#include <Volcano/Viewport.hpp>

VOLCANO_BEGIN

// ViewportRenderer

class ViewportRenderer: public QQuickFramebufferObject::Renderer
{
public:
    ViewportRenderer(const Viewport *viewport);
    ~ViewportRenderer(void) override;

public:
    bool init(void);
    void render(void) override;
    void synchronize(QQuickFramebufferObject *fb) override;

private:
    const Viewport *m_viewport;
    OpenGL::RendererPtr m_renderer;
    OpenGL::View m_view[2];
    OpenGL::Target m_target;
    int m_rendererViewSlot;
};

ViewportRenderer::ViewportRenderer(const Viewport *viewport):
    m_viewport(viewport),
    m_rendererViewSlot(0)
{
}

ViewportRenderer::~ViewportRenderer(void)
{
}

bool ViewportRenderer::init(void)
{
    Q_ASSERT(m_renderer != nullptr);

    m_renderer = OpenGL::defaultRenderer();
    if (!m_renderer)
        return false;

    m_rendererViewSlot = 0;

    return true;
}

void ViewportRenderer::render(void)
{
    Q_ASSERT(m_renderer != nullptr);

    m_renderer->render(m_view[m_rendererViewSlot], m_target);

    Q_ASSERT(m_viewport != nullptr);
    m_viewport->window()->resetOpenGLState();
}

void ViewportRenderer::synchronize(QQuickFramebufferObject *)
{
    Node *target = m_viewport->target();
    Camera *camera = m_viewport->camera();

    if (Q_UNLIKELY(target == nullptr))
        return;

    if (Q_UNLIKELY(camera == nullptr))
        return;

    // TODO Update 'm_target' if viewport size changed.

    // TODO Setup view matrix from camera.
    // m_renderer->

    // target->draw()
}

// Viewport

Viewport::Viewport(QQuickItem *parent):
    QQuickFramebufferObject(parent),
    m_fps(60),
    m_showCameras(false)
{
    m_frameTimer = startTimer(1000 / 60, Qt::PreciseTimer);
}

Viewport::~Viewport(void)
{
    if (m_frameTimer > 0)
        killTimer(m_frameTimer);
}

Viewport::Renderer *Viewport::createRenderer(void) const
{
    ViewportRenderer *r = new ViewportRenderer(this);
    if (r != nullptr)
        return nullptr;
    if (!r->init())
    {
        delete r;
        return nullptr;
    }
    return r;
}

void Viewport::setTarget(Node *node)
{
    if (m_target == node)
        return;

    m_target = node;
    targetChanged();
}

void Viewport::setCamera(Camera *camera)
{
    if (m_camera == camera)
        return;

    m_camera = camera;
    cameraChanged();
}

void Viewport::setFps(int v)
{
    if (m_fps == v)
        return;

    if (v < 1)
        return;

    if (m_frameTimer > 0)
        killTimer(m_frameTimer);

    m_fps = v;
    m_frameTimer = startTimer(1000 / v, Qt::PreciseTimer);

    fpsChanged();
}

void Viewport::setShowCameras(bool v)
{
    if (m_showCameras == v)
        return;

    m_showCameras = v;
    showCamerasChanged();
}

void Viewport::timerEvent(QTimerEvent *event)
{
    if (Q_LIKELY(event->timerId() == m_frameTimer))
        frame();
}

void Viewport::frame(void)
{
    // TODO build & swap view?

    update();
}

VOLCANO_END
