//
//
#ifndef VOLCANO_GAME_ENTITY_HPP
#define VOLCANO_GAME_ENTITY_HPP

#include <QList>
#include <QString>
#include <QVector3D>
#include <QQuaternion>
#include <QObject>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Component.hpp>

VOLCANO_GAME_BEGIN

using Components = QList<Component *>;

class VOLCANO_API Entity: public QObject
{
    Q_OBJECT

public:
    Entity(QObject *parent = nullptr);
    virtual ~Entity(void);

public:
    const QVector3D &position(void) const;
    void setPosition(const QVector3D &r);
    const QVector3D &scale(void) const;
    void setScale(const QVector3D &r);
    const QQuaternion &rotation(void) const;
    void setRotation(const QQuaternion &r);

private:
    QVector3D m_position;
    QVector3D m_scale;
    QQuaternion m_rotation;
    Components m_components;
};

VOLCANO_INLINE const QVector3D &Entity::position(void) const
{
    return m_position;
}

VOLCANO_INLINE void Entity::setPosition(const QVector3D &r)
{
    m_position = r;
}

VOLCANO_INLINE const QVector3D &Entity::scale(void) const
{
    return m_scale;
}

VOLCANO_INLINE void Entity::setScale(const QVector3D &r)
{
    m_scale = r;
}

VOLCANO_INLINE const QQuaternion &Entity::rotation(void) const
{
    return m_rotation;
}

VOLCANO_INLINE void Entity::setRotation(const QQuaternion &r)
{
    m_rotation = r;
}

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ENTITY_HPP
