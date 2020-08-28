//
//
#ifndef VOLCANO_WORLD_HPP
#define VOLCANO_WORLD_HPP

#include <QList>
#include <QVector3D>
#include <QObject>

#include <Volcano/Common.hpp>
#include <Volcano/Camera.hpp>
#include <Volcano/Entity.hpp>
#include <Volcano/Light.hpp>

VOLCANO_BEGIN

typedef QList<Entity *> EntityList;

class World: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)
    Q_PROPERTY(Light *ambient READ ambient)

public:
    World(QObject *parent = nullptr);
    ~World(void) override;

public:
    const QVector3D &gravity(void) const;
    void setGravity(const QVector3D &r);
    Light *ambient(void);
    void addEntity(Entity *p);
    void removeEntity(Entity *p);

signals:
    void gravityChanged(const QVector3D &r);
    void entityAdded(Entity *p);
    void entityRemoved(Entity *p);

protected:
    EntityList m_entityList;

private:
    QVector3D m_gravity;
    Light m_ambient;
};

VOLCANO_INLINE const QVector3D &World::gravity(void) const
{
    return m_gravity;
}

VOLCANO_INLINE Light *World::ambient(void)
{
    return &m_ambient;
}

VOLCANO_INLINE void World::addEntity(Entity *p)
{
    Q_ASSERT(!m_entityList.contains(p));
    m_entityList.append(p);
    entityAdded(p);
}

VOLCANO_INLINE void World::removeEntity(Entity *p)
{
    Q_ASSERT(m_entityList.contains(p));
    m_entityList.removeAll(p);
    entityRemoved(p);
}

VOLCANO_END

#endif // VOLCANO_WORLD_HPP
