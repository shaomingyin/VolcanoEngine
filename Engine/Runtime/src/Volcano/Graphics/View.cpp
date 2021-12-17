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
    m_transformStack.top().setTranslate(v);
}

void View::translateTo(const QVector3D &v)
{
    m_transformStack.top().setTranslateOffset(v);
}

void View::scale(const QVector3D &v)
{
    m_transformStack.top().setScale(v);
}

void View::scaleTo(const QVector3D &v)
{
    m_transformStack.top().setScaleOffset(v);
}

void View::rotate(const QQuaternion &v)
{
    m_transformStack.top().setRotation(v);
}

void View::rotate(const Rotation &v)
{
    rotate(QQuaternion::fromAxisAndAngle(v.axis(), v.angle()));
}

void View::rotateTo(const QQuaternion &v)
{
    m_transformStack.top().setRotationOffset(v);
}

void View::rotateTo(const Rotation &v)
{
    rotateTo(QQuaternion::fromAxisAndAngle(v.axis(), v.angle()));
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

void View::add(const Box *p)
{
}

void View::add(const Cylinder *p)
{

}

void View::add(const Sphere *p)
{

}

void View::add(const Plane *p)
{

}

void View::add(const Game::Light *p)
{

}

void View::add(const Game::DirectionalLight *p)
{

}

void View::add(const Game::PointLight *p)
{

}

void View::add(const Game::SpotLight *p)
{

}

void View::add(const Mesh *p)
{

}

const Transform &View::transform(void)
{
    return m_transformStack.top();
}

VOLCANO_GRAPHICS_END
