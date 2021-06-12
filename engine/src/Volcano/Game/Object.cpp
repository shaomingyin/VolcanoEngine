//
//
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

Object::Object(QObject *parent):
    QObject(parent)
{
}

Object::~Object(void)
{
}

bool Object::isEnabled(void) const
{
    return m_isEnabled;
}

void Object::setEnabled(bool v)
{
    if (m_isEnabled != v) {
        m_isEnabled = v;
        enabledChanged(v);
    }
}

const QVector3D &Object::position(void) const
{
    return m_position;
}

void Object::setPosition(const QVector3D &v)
{
    if (m_position != v) {
        m_position = v;
        positionChanged(v);
    }
}

const QVector3D &Object::scale(void) const
{
    return m_scale;
}

void Object::setScale(const QVector3D &v)
{
    if (m_scale != v) {
        m_scale = v;
        scaleChanged(v);
    }
}

const QQuaternion &Object::rotation(void) const
{
    return m_rotation;
}

void Object::setRotation(const QQuaternion &v)
{
    if (m_rotation != v) {
        m_rotation = v;
        rotationChanged(v);
    }
}

VOLCANO_GAME_END
