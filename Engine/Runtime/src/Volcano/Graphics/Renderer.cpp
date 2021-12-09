//
//
#include <QThread>
#include <QQuickOpenGLUtils>

#include <Volcano/Graphics/Context.hpp>
#include <Volcano/Graphics/CameraView.hpp>
#include <Volcano/Graphics/Renderer.hpp>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(void):
    m_gl(nullptr),
    m_isClearEnabled(true),
    m_clearColor(QColor::fromRgbF(0.0f, 0.0f, 0.0f)),
    m_position(0.0f, 0.0f, 0.0f),
    m_direction(0.0f, 0.0f, -1.0f),
    m_up(0.0f, 1.0f, 0.0f)
{
}

Renderer::~Renderer(void)
{
    // TODO release opengl resources...
}

bool Renderer::init(int width, int height)
{
    Q_ASSERT(m_gl == nullptr);

    m_gl = Context::current()->glFunctions();
    if (m_gl == nullptr)
        return false;

    return true;
}

void Renderer::reset(void)
{
    VisibleSet::reset();

    // TODO
}

void Renderer::resize(const QSize &v)
{
    if (Q_LIKELY(m_size == v))
        return;

    // TODO

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

void Renderer::enableClear(void)
{
    m_isClearEnabled = true;
}

void Renderer::disableClear(void)
{
    m_isClearEnabled = false;
}

void Renderer::setClearColor(const QColor &v)
{
    m_clearColor = v;
}

void Renderer::lookAt(const QVector3D &position, const QVector3D &direction, const QVector3D &up)
{
    m_position = position;
    m_direction = direction;
    m_up = up;
}

VOLCANO_GRAPHICS_END
