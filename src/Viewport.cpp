//
//
#include <Volcano/OpenGLRenderer.hpp>
#include <Volcano/OpenGLMemory.hpp>
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

private:
    const Viewport *m_viewport;
    OpenGLMemory *m_memory;
    OpenGLRenderer *m_renderer;
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
}

void ViewportRenderer::synchronize(QQuickFramebufferObject *)
{
    if (Q_UNLIKELY(!tryInit()))
        return;

    m_renderer->reset(m_viewport->width(), m_viewport->height());

    // TODO Build visible set to 'm_renderer' from 'm_viewport'.
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

    return true;
}

// Viewport

Viewport::Viewport(QQuickItem *parent):
    QQuickFramebufferObject(parent)
{
}

Viewport::~Viewport(void)
{
}

Viewport::Renderer *Viewport::createRenderer(void) const
{
    return (new ViewportRenderer(this));
}

void Viewport::setScene(Scene *scene)
{
    if (m_scene == scene)
        return;

    m_scene = scene;
    sceneChanged();
}

void Viewport::setCamera(Camera *camera)
{
    if (m_camera == camera)
        return;

    m_camera = camera;
    cameraChanged();
}

VOLCANO_END
