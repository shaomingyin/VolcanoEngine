//
//
#include <QQuickWindow>

#include <Volcano/OpenGLRenderer.hpp>
#include <Volcano/OpenGLMemory.hpp>
#include <Volcano/Camera.hpp>
#include <Volcano/DirectionalLight.hpp>
#include <Volcano/Entity.hpp>
#include <Volcano/Mesh.hpp>
#include <Volcano/PointLight.hpp>
#include <Volcano/Scene.hpp>
#include <Volcano/SpotLight.hpp>
#include <Volcano/View.hpp>
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
    void processNode(const Node *node, bool recursive);
    bool processOneNode(const Node *node);
    bool processCamera(const Camera *camera);
    bool processDirectionLight(const DirectionalLight *light);
    bool processEntity(const Entity *entity);
    bool processMesh(const Mesh *mesh);
    bool processPointLight(const PointLight *light);
    bool processScene(const Scene *scene);
    bool processSpotLight(const SpotLight *light);

private:
    const Viewport *m_viewport;
    OpenGLMemory *m_memory;
    OpenGLRenderer *m_renderer;
    View m_view[2];
    int m_renderingViewSlot;
};

ViewportRenderer::ViewportRenderer(const Viewport *viewport):
    m_viewport(viewport),
    m_memory(nullptr),
    m_renderer(nullptr)
{
}

ViewportRenderer::~ViewportRenderer(void)
{
    if (m_memory != nullptr)
        delete m_memory;
    if (m_renderer != nullptr)
        delete m_renderer;
}

void ViewportRenderer::render(void)
{
    if (Q_UNLIKELY(m_renderer == nullptr))
        return;

    m_renderer->render();

    m_viewport->window()->resetOpenGLState();
}

void ViewportRenderer::synchronize(QQuickFramebufferObject *)
{
    if (Q_UNLIKELY(!tryInit()))
        return;

    m_renderer->reset(m_viewport->width(), m_viewport->height());

    if (Q_UNLIKELY(m_viewport->targetRO() == nullptr))
        return;

    const Camera *camera = m_viewport->cameraRO();
    if (Q_UNLIKELY(camera == nullptr))
        return;

    // TODO Setup view matrix to renderer.
    // m_renderer->

    processNode(m_viewport->targetRO(), m_viewport->recursive());
}

bool ViewportRenderer::tryInit(void)
{
    if (Q_LIKELY(m_renderer != nullptr))
        return true;

    m_renderer = new OpenGLRenderer();
    if (m_renderer == nullptr)
        return false;

    if (!m_renderer->init())
    {
        delete m_renderer;
        m_renderer = nullptr;
        return false;
    }

    m_memory = new OpenGLMemory();
    if (m_memory == nullptr)
    {
        delete m_renderer;
        m_renderer = nullptr;
        return false;
    }

    m_renderingViewSlot = 0;

    return true;
}

void ViewportRenderer::processNode(const Node *node, bool recursive)
{
    Q_ASSERT(node != nullptr);

    bool visible = processOneNode(node);

    if (visible && recursive)
    {
        const Node::Nodes &children = node->subNodesRO();
        for (auto it(children.begin()); it != children.end(); ++it)
            processNode(*it, true);
    }
}

bool ViewportRenderer::processOneNode(const Node *node)
{
    const Camera *camera = qobject_cast<const Camera *>(node);
    if (camera != nullptr)
        return processCamera(camera);

    const DirectionalLight *dirLight = qobject_cast<const DirectionalLight *>(node);
    if (dirLight != nullptr)
        return processDirectionLight(dirLight);

    const Entity *entity = qobject_cast<const Entity *>(node);
    if (entity != nullptr)
        return processEntity(entity);

    const Mesh *mesh = qobject_cast<const Mesh *>(node);
    if (mesh != nullptr)
        return processMesh(mesh);

    const PointLight *pointLight = qobject_cast<const PointLight *>(node);
    if (pointLight != nullptr)
        return processPointLight(pointLight);

    const Scene *scene = qobject_cast<const Scene *>(node);
    if (scene != nullptr)
        return processScene(scene);

    const SpotLight *spotLight = qobject_cast<const SpotLight *>(node);
    if (spotLight != nullptr)
        return processSpotLight(spotLight);

    return false;
}

bool ViewportRenderer::processCamera(const Camera *camera)
{
    if (!m_viewport->showCameras())
        return true;

    return true;
}

bool ViewportRenderer::processDirectionLight(const DirectionalLight *light)
{
    return true;
}

bool ViewportRenderer::processEntity(const Entity *entity)
{
    return true;
}

bool ViewportRenderer::processMesh(const Mesh *mesh)
{
    return true;
}

bool ViewportRenderer::processPointLight(const PointLight *light)
{
    return true;
}

bool ViewportRenderer::processScene(const Scene *scene)
{
    return true;
}

bool ViewportRenderer::processSpotLight(const SpotLight *light)
{
    return true;
}

// Viewport

Viewport::Viewport(QQuickItem *parent):
    QQuickFramebufferObject(parent),
    m_fps(60),
    m_recursive(true),
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

void Viewport::setRecursive(bool v)
{
    if (m_recursive == v)
        return;

    m_recursive = v;
    recursiveChanged();
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
        update();
}

VOLCANO_END
