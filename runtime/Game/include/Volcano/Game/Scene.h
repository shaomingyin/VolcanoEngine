//
//
#ifndef VOLCANO_GAME_SCENE_H
#define VOLCANO_GAME_SCENE_H

#include <QList>
#include <QQmlListProperty>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Entity.h>
#include <Volcano/Game/Object.h>

VOLCANO_GAME_BEGIN

class Scene: public Object {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Entity> entities READ qmlEntities)
    Q_CLASSINFO("DefaultProperty", "entities")

public:
    Scene(QObject* parent = nullptr);
    ~Scene() override;

public:
    const QList<Entity*>& entities() const {
        return entities_;
    }

    void update(Clock::duration elapsed);
    void appendEntity(Entity* p);
    Entity* entityAt(qsizetype i);
    void clearEntities();
    qsizetype entityCount();
    void removeLastEntity();
    void replaceEntity(qsizetype i, Entity* p);
    QQmlListProperty<Entity> qmlEntities();

signals:
    void entityAdded(Entity* p);
    void entityRemoved(Entity* p);
    void componentAdded(Entity* entity, Component *component);
    void componentRemoved(Entity* entity, Component *component);

private:
    void onEntityAdded(Entity* entity);
    void onComponentAdded(Entity* entity, Component* component);
    void onEntityRemoved(Entity* entity);
    void onComponentRemoved(Entity* entity, Component* component);

private:
    QList<Entity*> entities_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SCENE_H
