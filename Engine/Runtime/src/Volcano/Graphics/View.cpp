//
//
#include <QThread>
#include <QQuickOpenGLUtils>

#include <Volcano/Graphics/Context.hpp>
#include <Volcano/Graphics/CameraView.hpp>
#include <Volcano/Graphics/View.hpp>

VOLCANO_GRAPHICS_BEGIN

View::View(void):
    m_gl(nullptr),
    m_isClearEnabled(true),
    m_clearColor(QColor::fromRgbF(0.0f, 0.0f, 0.0f)),
    m_position(0.0f, 0.0f, 0.0f),
    m_direction(0.0f, 0.0f, -1.0f),
    m_up(0.0f, 1.0f, 0.0f)
{
}

View::~View(void)
{
    // TODO release opengl resources...
}

bool View::init(int width, int height)
{
    Q_ASSERT(m_gl == nullptr);

    m_gl = Context::current()->glFunctions();
    if (m_gl == nullptr)
        return false;

    return true;
}

void View::reset(void)
{
    VisibleSet::reset();

    // TODO
}

void View::resize(const QSize &v)
{
    if (Q_LIKELY(m_size == v))
        return;

    // TODO

    m_size = v;
}

void View::resize(int width, int height)
{
    resize(QSize(width, height));
}

void View::render(void)
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

void View::enableClear(void)
{
    m_isClearEnabled = true;
}

void View::disableClear(void)
{
    m_isClearEnabled = false;
}

void View::setClearColor(const QColor &v)
{
    m_clearColor = v;
}

void View::lookAt(const QVector3D &position, const QVector3D &direction, const QVector3D &up)
{
    m_position = position;
    m_direction = direction;
    m_up = up;
}

VOLCANO_GRAPHICS_END
