//
//
#include <Volcano/Renderer.h>
#include <Volcano/Viewport.h>

VOLCANO_BEGIN

// ViewportRenderer

class ViewportRenderer: public Viewport::Renderer
{
public:
    ViewportRenderer(void);
    ~ViewportRenderer(void) override;

public:
    void render(void) override;
    void synchronize(QQuickFramebufferObject *obj) override;

protected:
    void load(Node *root);

private:
    Volcano::Renderer::View m_view;
    Node *m_root;
};

ViewportRenderer::ViewportRenderer(void):
    m_root(nullptr)
{
    m_view.setClear(true);
}

ViewportRenderer::~ViewportRenderer(void)
{
}

void ViewportRenderer::render(void)
{
    Volcano::Renderer::Context *context = Volcano::Renderer::Context::current();
    if (Q_LIKELY(context != nullptr))
        context->render(m_view);
}

void ViewportRenderer::synchronize(QQuickFramebufferObject *obj)
{
    Volcano::Renderer::Context *context = Volcano::Renderer::Context::current();
    if (Q_UNLIKELY(context == nullptr))
        return;

    Viewport *viewport = reinterpret_cast<Viewport *>(obj);
    if (Q_UNLIKELY(m_root != viewport->target()))
        load(viewport->target());

    m_view.reset();
    m_view.setViewMatrix(viewport->camera()->viewMatrix());

    //QMatrix4x4 m;
    //m.Perspective(viewport->camera()->fov(), )
    //m_view.setProjectMatrix(viewport->camera()->)
}

void ViewportRenderer::load(Node *root)
{
    Q_ASSERT(m_root != root);

    m_root = root;
    if (m_root == nullptr)
        return;

    // TODO load resource to OpenGL...
}

// Viewport

Viewport::Viewport(QQuickItem *parent):
    QQuickFramebufferObject(parent),
    m_target(nullptr),
    m_camera(nullptr)
{
}

Viewport::~Viewport(void)
{
}

void Viewport::setTarget(Node *p)
{
    if (m_target != p)
    {
        m_target = p;
        targetChanged(p);
    }
}

void Viewport::setCamera(Camera *p)
{
    if (m_camera != p)
    {
        m_camera = p;
        cameraChanged(p);
    }
}

Viewport::Renderer *Viewport::createRenderer(void) const
{
    return (new ViewportRenderer());
}

VOLCANO_END
