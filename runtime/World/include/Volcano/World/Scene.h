//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <QList>
#include <QQmlListProperty>
#include <QObject>

#include <Volcano/World/Common.h>
#include <Volcano/World/Light.h>
#include <Volcano/World/Entity.h>

VOLCANO_WORLD_BEGIN

class Scene: public QObject {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Entity> entities READ qmlEntities)
    Q_CLASSINFO("DefaultProperty", "entities")

public:
    Scene(QObject* parent = nullptr);
    ~Scene() override;

public:
    Light* ambientLight() noexcept {
        return &ambient_light_;
    }

    const QList<Entity*>& entities() const {
        return entities_;
    }

    void appendEntity(Entity* p);
    Entity* entityAt(qsizetype i);
    void clearEntities();
    qsizetype entityCount();
    void removeLastEntity();
    void replaceEntity(qsizetype i, Entity* p);
    QQmlListProperty<Entity> qmlEntities();
    void update(Clock::duration elapsed);

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
    Light ambient_light_;
    QList<Entity*> entities_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCENE_H
