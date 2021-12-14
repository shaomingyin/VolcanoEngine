//
//
#include <memory>

#include <QThread>
#include <QQuickOpenGLUtils>

#include <Volcano/Graphics/Context.hpp>
#include <Volcano/Graphics/CameraView.hpp>
#include <Volcano/Graphics/Renderer.hpp>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(void):
    m_gl(nullptr),
    m_fbo(nullptr)
{
}

Renderer::~Renderer(void)
{
    if (m_fbo != nullptr)
        delete m_fbo;
}

bool Renderer::init(int width, int height)
{
    Q_ASSERT(m_gl == nullptr);
    m_gl = Context::current()->glFunctions();
    if (m_gl == nullptr)
        return false;

    reset();
    resize(width, height);

    return true;
}

void Renderer::reset(void)
{
    View::reset();

    // TODO
}

void Renderer::resize(const QSize &v)
{
    if (Q_LIKELY(m_size == v))
        return;

    if (m_fbo != nullptr) {
        delete m_fbo;
        m_fbo = nullptr;
    }

    auto fbo = std::make_unique<QOpenGLFramebufferObject>(v.width(), v.height());
    if (!fbo || !fbo->isValid())
        return;

    // TODO

    m_fbo = fbo.release();
    m_size = v;
}

void Renderer::resize(int width, int height)
{
    resize(QSize(width, height));
}

void Renderer::render(void)
{
    Q_ASSERT(m_gl != nullptr);

    if (m_isClearEnabled) {
        m_gl->glClearColor(m_clearColor.redF(),
                           m_clearColor.greenF(),
                           m_clearColor.blueF(),
                           m_clearColor.alphaF());
        m_gl->glClear(GL_COLOR_BUFFER_BIT);
    }

    // TODO
}

VOLCANO_GRAPHICS_END
