//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <QSet>
#include <QList>
#include <QObject>

#include <Volcano/World/Common.h>
#include <Volcano/World/Entity.h>
#include <Volcano/World/Dynamic.h>
#include <Volcano/World/Camera.h>
#include <Volcano/World/AmbientLight.h>
#include <Volcano/World/Object.h>

VOLCANO_WORLD_BEGIN

class Scene: public Object {
    Q_OBJECT
    Q_PROPERTY(Camera* camera READ camera FINAL)
    Q_PROPERTY(AmbientLight* ambientLight READ ambientLight FINAL)
    Q_PROPERTY(Dynamic* dynamic READ dynamic FINAL)
    Q_PROPERTY(QQmlListProperty<Entity> entities READ qmlEntities)
    Q_CLASSINFO("DefaultProperty", "entities")

public:
    Scene(QObject* parent = nullptr);
    ~Scene() override;

public:
    Camera* camera() {
        return &camera_;
    }

    AmbientLight* ambientLight() {
        return &ambient_light_;
    }

    Dynamic* dynamic() {
        return &dynamic_;
    }

    const QList<Entity*>& entities() const {
        return entities_;
    }

    void update(Duration elapsed);
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
    Camera camera_;
    AmbientLight ambient_light_;
    Dynamic dynamic_;
    QList<Entity*> entities_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCENE_H
