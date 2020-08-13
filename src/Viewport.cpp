//
//
#include <QQuickWindow>

#include <Volcano/Camera.hpp>
#include <Volcano/DirectionalLight.hpp>
#include <Volcano/Entity.hpp>
#include <Volcano/Mesh.hpp>
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
    void render(void) override;
    void synchronize(QQuickFramebufferObject *fb) override;

private:
    bool tryInit(void);
    void processNode(const Node *node);
    void add(OpenGL::View &view, const Camera *camera);
    void add(OpenGL::View &view, const DirectionalLight *light);
    void add(OpenGL::View &view, const Entity *entity);
    void add(OpenGL::View &view, const Mesh *mesh);
    void add(OpenGL::View &view, const PointLight *light);
    void add(OpenGL::View &view, const Scene *scene);
    void add(OpenGL::View &view, const SpotLight *light);

private:
    const Viewport *m_viewport;
    OpenGL::RendererPtr m_renderer;
    OpenGL::View m_view[2];
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

void ViewportRenderer::render(void)
{
    if (Q_UNLIKELY(!m_renderer))
        return;

    m_renderer->render(m_view[m_rendererViewSlot]);
    m_viewport->window()->resetOpenGLState();
}

void ViewportRenderer::synchronize(QQuickFramebufferObject *)
{
    if (Q_UNLIKELY(!tryInit()))
        return;

    Node *target = m_viewport->target();
    Camera *camera = m_viewport->camera();

    if (Q_UNLIKELY(target == nullptr))
        return;

    if (Q_UNLIKELY(camera == nullptr))
        return;

    // TODO Setup view matrix from camera.
    // m_renderer->

    processNode(target);
}

bool ViewportRenderer::tryInit(void)
{
    if (Q_LIKELY(m_renderer))
        return true;

    auto renderer = OpenGL::defaultRenderer();
    if (!renderer)
        return false;

    m_renderer = renderer;
    m_rendererViewSlot = 0;

    return true;
}

void ViewportRenderer::processNode(const Node *node)
{
    Q_ASSERT(node != nullptr);

    OpenGL::View &view = m_view[!m_rendererViewSlot];
    view.reset();

    do
    {
        const Entity *entity = qobject_cast<const Entity *>(node);
        if (entity != nullptr)
        {
            add(view, entity);
            break;
        }

        const Mesh *mesh = qobject_cast<const Mesh *>(node);
        if (mesh != nullptr)
        {
            add(view, mesh);
            break;
        }

        const DirectionalLight *dirLight = qobject_cast<const DirectionalLight *>(node);
        if (dirLight != nullptr)
        {
            add(view, dirLight);
            break;
        }

        const PointLight *pointLight = qobject_cast<const PointLight *>(node);
        if (pointLight != nullptr)
        {
            add(view, pointLight);
            break;
        }

        const SpotLight *spotLight = qobject_cast<const SpotLight *>(node);
        if (spotLight != nullptr)
        {
            add(view, spotLight);
            break;
        }

        const Scene *scene = qobject_cast<const Scene *>(node);
        if (scene != nullptr)
        {
            add(view, scene);
            break;
        }

        const Camera *camera = qobject_cast<const Camera *>(node);
        if (camera != nullptr)
        {
            add(view, camera);
            break;
        }
    }
    while (0);

    const Node::Nodes &children = node->subNodes();
    for (auto it(children.begin()); it != children.end(); ++it)
        processNode(*it);
}

void ViewportRenderer::add(OpenGL::View &view, const Camera *camera)
{
    if (!m_viewport->showCameras())
        return;
}

void ViewportRenderer::add(OpenGL::View &view, const DirectionalLight *light)
{
}

void ViewportRenderer::add(OpenGL::View &view, const Entity *entity)
{
}

void ViewportRenderer::add(OpenGL::View &view, const Mesh *mesh)
{
}

void ViewportRenderer::add(OpenGL::View &view, const PointLight *light)
{
}

void ViewportRenderer::add(OpenGL::View &view, const Scene *scene)
{
}

void ViewportRenderer::add(OpenGL::View &view, const SpotLight *light)
{
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
    return (new ViewportRenderer(this));
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
    if (Q_LIKELY(m_target != nullptr))
        m_target->tick(0.016f);

    // TODO build & swap view?

    update();
}

VOLCANO_END
