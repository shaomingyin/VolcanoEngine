//
//
#include <Volcano/Transform.hpp>

VOLCANO_BEGIN

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

const QVector3D &Transform::translate(void) const
{
    return m_translate;
}

void Transform::setTranslate(const QVector3D &v)
{
    m_translate = v;
}

void Transform::setTranslate(float x, float y, float z)
{
    setTranslate(QVector3D(x, y, z));
}

void Transform::setTranslateOffset(const QVector3D &v)
{
    m_translate += v;
}

void Transform::setTrnaslateOffset(float x, float y, float z)
{
    setTranslateOffset(QVector3D(x, y, z));
}

const QVector3D &Transform::scale(void) const
{
    return m_scale;
}

void Transform::setScale(const QVector3D &v)
{
    m_scale = v;
}

void Transform::setScale(float x, float y, float z)
{
    setScale(QVector3D(x, y, z));
}

void Transform::setScaleOffset(const QVector3D &v)
{
    m_scale *= v;
}

void Transform::setScaleOffset(float x, float y, float z)
{
    setScaleOffset(QVector3D(x, y, z));
}

const QQuaternion &Transform::rotation(void) const
{
    return m_rotation;
}

void Transform::setRotation(const QQuaternion &v)
{
    m_rotation = v;
}

void Transform::setRotation(float angle, const QVector3D &axis)
{
    setRotation(QQuaternion::fromAxisAndAngle(axis, angle));
}

void Transform::setRotation(float angle, float x, float y, float z)
{
    setRotation(QQuaternion::fromAxisAndAngle(x, y, z, angle));
}

void Transform::setRotationOffset(const QQuaternion &v)
{
    m_rotation *= v;
}

void Transform::setRotationOffset(float angle, const QVector3D &axis)
{
    setRotationOffset(QQuaternion::fromAxisAndAngle(axis, angle));
}

void Transform::setRotationOffset(float angle, float x, float y, float z)
{
    setRotationOffset(QQuaternion::fromAxisAndAngle(x, y, z, angle));
}

void Transform::buildMatrix4x4(QMatrix4x4 &out)
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

bool Transform::operator==(const Transform &that) const
{
    return (qFuzzyCompare(m_translate, that.m_translate) &&
            qFuzzyCompare(m_scale, that.m_scale) &&
            qFuzzyCompare(m_rotation, that.m_rotation));
}

bool Transform::operator!=(const Transform &that) const
{
    return !operator==(that);
}

VOLCANO_END
