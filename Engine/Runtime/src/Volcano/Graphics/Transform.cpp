//
//
#include <Volcano/Graphics/Transform.hpp>

VOLCANO_GRAPHICS_BEGIN

Transform::Transform(void)
{
    reset();
}

Transform::Transform(const Transform &that):
    m_translate(that.m_translate),
    m_scale(that.m_scale),
    m_rotation(that.m_rotation)
{
}

Transform::Transform(Transform &&that):
    m_translate(that.m_translate),
    m_scale(that.m_scale),
    m_rotation(that.m_rotation)
{
}

Transform::Transform(const QVector3D &translate, const QVector3D &scale, const QQuaternion &rotation):
    m_translate(translate),
    m_scale(scale),
    m_rotation(rotation)
{
}

void Transform::reset(void)
{
    m_translate = QVector3D(0.0f, 0.0f, 0.0f);
    m_scale = QVector3D(1.0f, 1.0f, 1.0f);
    m_rotation = QQuaternion::fromAxisAndAngle(0.0f, 0.0f, 0.0f, 0.0f);
}

void Transform::translate(const QVector3D &v)
{
    m_translate = v;
}

void Transform::translate(float x, float y, float z)
{
    translate(QVector3D(x, y, z));
}

void Transform::translateTo(const QVector3D &v)
{
    m_translate += v;
}

void Transform::trnaslateTo(float x, float y, float z)
{
    translateTo(QVector3D(x, y, z));
}

void Transform::scale(const QVector3D &v)
{
    m_scale = v;
}

void Transform::scale(float x, float y, float z)
{
    scale(QVector3D(x, y, z));
}

void Transform::scaleTo(const QVector3D &v)
{
    m_scale *= v;
}

void Transform::scaleTo(float x, float y, float z)
{
    scaleTo(QVector3D(x, y, z));
}

void Transform::rotate(const QQuaternion &v)
{
    m_rotation = v;
}

void Transform::rotate(float angle, const QVector3D &axis)
{
    rotate(QQuaternion::fromAxisAndAngle(axis, angle));
}

void Transform::rotate(float angle, float x, float y, float z)
{
    rotate(QQuaternion::fromAxisAndAngle(x, y, z, angle));
}

void Transform::rotateTo(const QQuaternion &v)
{
    m_rotation *= v;
}

void Transform::rotateTo(float angle, const QVector3D &axis)
{
    rotateTo(QQuaternion::fromAxisAndAngle(axis, angle));
}

void Transform::rotateTo(float angle, float x, float y, float z)
{
    rotateTo(QQuaternion::fromAxisAndAngle(x, y, z, angle));
}

void Transform::toMatrix(QMatrix4x4 &out)
{
    out.setToIdentity();
    out.scale(m_scale);
    out.rotate(m_rotation);
    out.translate(m_translate);
}

Transform &Transform::operator=(const Transform &that)
{
    m_translate = that.m_translate;
    m_scale = that.m_scale;
    m_rotation = that.m_rotation;

    return (*this);
}

Transform &Transform::operator=(Transform &&that)
{
    m_translate = that.m_translate;
    m_scale = that.m_scale;
    m_rotation = that.m_rotation;

    return (*this);
}

bool Transform::operator==(const Transform &that) const
{
    return (qFuzzyCompare(m_translate, that.m_translate) &&
            qFuzzyCompare(m_scale, that.m_scale) &&
            qFuzzyCompare(m_rotation, that.m_rotation));
}

bool Transform::operator!=(const Transform &that) const
{
    return !((*this) == that);
}

VOLCANO_GRAPHICS_END
