//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <QList>
#include <QQmlListProperty>
#include <QObject>

#include <Volcano/World/Common.h>
#include <Volcano/World/Entity.h>
#include <Volcano/World/Physics.h>

VOLCANO_WORLD_BEGIN

using EntityList = QList<Entity*>;

class Scene: public QObject {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Entity> entities READ qmlEntities)
    Q_PROPERTY(Physics* physics READ physics)
    Q_CLASSINFO("DefaultProperty", "entities")

public:
    Scene(QObject* parent = nullptr);
    ~Scene() override;

public:
    const EntityList& entities() const noexcept;
    void appendEntity(Entity* p);
    Entity* entityAt(qsizetype i);
    void clearEntities();
    qsizetype entityCount();
    void removeLastEntity();
    void replaceEntity(qsizetype i, Entity* p);
    QQmlListProperty<Entity> qmlEntities();
    Physics* physics() noexcept;
    void update(Clock::duration deltaTime, int collisionSteps = 1);

signals:
    void entityAdded(Entity* p);
    void entityRemoved(Entity* p);
    void componentAdded(Entity* entity, QObject *component);
    void componentRemoved(Entity* entity, QObject *component);

private:
    void handleEntityAdded(Entity* entity);
    void handleComponentAdded(Entity* entity, QObject* component);
    void handleEntityRemoved(Entity* entity);
    void handleComponentRemoved(Entity* entity, QObject* component);

private:
    EntityList entities_;
    Physics physics_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCENE_H
