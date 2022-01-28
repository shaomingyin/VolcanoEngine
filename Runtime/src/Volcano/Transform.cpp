//
//
#include <Volcano/Transform.hpp>

VOLCANO_BEGIN

// Transform::Data

Transform::Data::Data(void)
{
}

Transform::Data::Data(const Data &that):
    m_translate(that.m_translate),
    m_scale(that.m_scale),
    m_rotation(that.m_rotation)
{
}

Transform::Data::Data(const QVector3D &translate, const QVector3D &scale, const QQuaternion &rotation):
    m_translate(translate),
    m_scale(scale),
    m_rotation(rotation)
{
}

void Transform::Data::reset(void)
{
    m_translate = QVector3D(0.0f, 0.0f, 0.0f);
    m_scale = QVector3D(1.0f, 1.0f, 1.0f);
    m_rotation = QQuaternion::fromAxisAndAngle(0.0f, 0.0f, 0.0f, 0.0f);
}

const QVector3D &Transform::Data::translate(void) const
{
    return m_translate;
}

void Transform::Data::setTranslate(const QVector3D &v)
{
    m_translate = v;
}

void Transform::Data::setTranslate(float x, float y, float z)
{
    setTranslate(QVector3D(x, y, z));
}

void Transform::Data::setTranslateOffset(const QVector3D &v)
{
    m_translate += v;
}

void Transform::Data::setTrnaslateOffset(float x, float y, float z)
{
    setTranslateOffset(QVector3D(x, y, z));
}

const QVector3D &Transform::Data::scale(void) const
{
    return m_scale;
}

void Transform::Data::setScale(const QVector3D &v)
{
    m_scale = v;
}

void Transform::Data::setScale(float x, float y, float z)
{
    setScale(QVector3D(x, y, z));
}

void Transform::Data::setScaleOffset(const QVector3D &v)
{
    m_scale *= v;
}

void Transform::Data::setScaleOffset(float x, float y, float z)
{
    setScaleOffset(QVector3D(x, y, z));
}

const QQuaternion &Transform::Data::rotation(void) const
{
    return m_rotation;
}

void Transform::Data::setRotation(const QQuaternion &v)
{
    m_rotation = v;
}

void Transform::Data::setRotation(float angle, const QVector3D &axis)
{
    setRotation(QQuaternion::fromAxisAndAngle(axis, angle));
}

void Transform::Data::setRotation(float angle, float x, float y, float z)
{
    setRotation(QQuaternion::fromAxisAndAngle(QVector3D(x, y, z), angle));
}

void Transform::Data::setRotationOffset(const QQuaternion &v)
{
    m_rotation *= v;
}

void Transform::Data::setRotationOffset(float angle, const QVector3D &axis)
{
    setRotationOffset(QQuaternion::fromAxisAndAngle(axis, angle));
}

void Transform::Data::setRotationOffset(float angle, float x, float y, float z)
{
    setRotationOffset(QQuaternion::fromAxisAndAngle(QVector3D(x, y, z), angle));
}

void Transform::Data::buildMatrix4x4(QMatrix4x4 &out)
{
    out.setToIdentity();
    out.scale(m_scale);
    out.rotate(m_rotation);
    out.translate(m_translate);
}

Transform::Data &Transform::Data::operator=(const Data &that)
{
    m_translate = that.m_translate;
    m_scale = that.m_scale;
    m_rotation = that.m_rotation;

    return (*this);
}

bool Transform::Data::operator==(const Data &that) const
{
    return qFuzzyCompare(m_translate, that.m_translate) &&
            qFuzzyCompare(m_scale, that.m_scale) &&
            qFuzzyCompare(m_rotation, that.m_rotation);
}

bool Transform::Data::operator!=(const Data &that) const
{
    return !operator==(that);
}

// Transform

Transform::Transform(QObject *parent):
    QObject(parent)
{
    reset();
}

Transform::Transform(const Data &data, QObject *parent):
    QObject(parent),
    m_data(data)
{
}

Transform::Transform(const QVector3D &translate, const QVector3D &scale, const QQuaternion &rotation, QObject *parent):
    QObject(parent),
    m_data(translate, scale, rotation)
{
}

const Transform::Data &Transform::data(void) const
{
    return m_data;
}

void Transform::setData(const Data &v)
{
    m_data = v;
    emit translateChanged(m_data.translate());
    emit scaleChanged(m_data.scale());
    emit rotationChanged(m_data.rotation());
}

void Transform::reset(void)
{
    m_data.reset();
    emit translateChanged(m_data.translate());
    emit scaleChanged(m_data.scale());
    emit rotationChanged(m_data.rotation());
}

const QVector3D &Transform::translate(void) const
{
    return m_data.translate();
}

void Transform::setTranslate(const QVector3D &v)
{
    if (!qFuzzyCompare(m_data.translate(), v)) {
        m_data.setTranslate(v);
        emit translateChanged(v);
    }
}

void Transform::setTranslate(float x, float y, float z)
{
    setTranslate(QVector3D(x, y, z));
}

void Transform::setTranslateOffset(const QVector3D &v)
{
    m_data.setTranslateOffset(v);
    emit translateChanged(m_data.translate());
}

void Transform::setTrnaslateOffset(float x, float y, float z)
{
    setTranslateOffset(QVector3D(x, y, z));
}

const QVector3D &Transform::scale(void) const
{
    return m_data.scale();
}

void Transform::setScale(const QVector3D &v)
{
    if (!qFuzzyCompare(m_data.scale(), v)) {
        m_data.setScale(v);
        emit scaleChanged(v);
    }
}

void Transform::setScale(float x, float y, float z)
{
    setScale(QVector3D(x, y, z));
}

void Transform::setScaleOffset(const QVector3D &v)
{
    m_data.setScaleOffset(v);
    emit scaleChanged(m_data.scale());
}

void Transform::setScaleOffset(float x, float y, float z)
{
    setScaleOffset(QVector3D(x, y, z));
}

const QQuaternion &Transform::rotation(void) const
{
    return m_data.rotation();
}

void Transform::setRotation(const QQuaternion &v)
{
    if (!qFuzzyCompare(m_data.rotation(), v)) {
        m_data.setRotation(v);
        emit rotationChanged(v);
    }
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
    m_data.setRotationOffset(v);
    emit rotationChanged(m_data.rotation());
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
    m_data.buildMatrix4x4(out);
}

VOLCANO_END
