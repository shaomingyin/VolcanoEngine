//
//
#include <Volcano/Graphics/View.hpp>

VOLCANO_GRAPHICS_BEGIN

View::View(void)
{
    reset();
}

View::~View(void)
{
}

void View::reset(void)
{
    m_isClearEnabled = true;
    m_clearColor = QColor::fromRgb(0, 0, 0);

    m_position = QVector3D(0.0f, 0.0f, 0.0f);
    m_direction = QVector3D(0.0f, 0.0f, -1.0f);
    m_up = QVector3D(0.0f, 1.0f, 0.0f);

    m_fov = 90.0f;
    m_ratio = 16.0f / 9.0f;
    m_nearPlane = -0.1f;
    m_farPlane = -1000.0f;

    m_transformStack.resize(1);
    m_transformStack.first().reset();
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

void View::perspective(float fov, float ratio, float nearPlane, float farPlane)
{
    Q_ASSERT(nearPlane < 0.0f);
    Q_ASSERT(farPlane < 0.0f);
    Q_ASSERT(farPlane < nearPlane);

    m_fov = fov;
    m_ratio = ratio;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
}

void View::translate(const QVector3D &v)
{
    m_transformStack.top().translate(v);
}

void View::translateTo(const QVector3D &v)
{
    m_transformStack.top().translateTo(v);
}

void View::scale(const QVector3D &v)
{
    m_transformStack.top().scale(v);
}

void View::scaleTo(const QVector3D &v)
{
    m_transformStack.top().scaleTo(v);
}

void View::rotate(const QQuaternion &v)
{
    m_transformStack.top().rotate(v);
}

void View::rotateTo(const QQuaternion &v)
{
    m_transformStack.top().rotateTo(v);
}

void View::push(bool copyTop)
{
    if (!copyTop)
        m_transformStack.emplaceBack();
    else
        m_transformStack.push(m_transformStack.top());
}

void View::pop(void)
{
    Q_ASSERT(m_transformStack.size() > 1);
    m_transformStack.pop();
}

Transform &View::currentTransform(void)
{
    return m_transformStack.top();
}

VOLCANO_GRAPHICS_END
