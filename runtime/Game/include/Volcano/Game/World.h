//
//
#ifndef VOLCANO_GAME_WORLD_H
#define VOLCANO_GAME_WORLD_H

#include <QSet>
#include <QList>
#include <QObject>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Entity.h>
#include <Volcano/Game/Dynamic.h>
#include <Volcano/Game/Camera.h>
#include <Volcano/Game/AmbientLight.h>
#include <Volcano/Game/Object.h>

VOLCANO_GAME_BEGIN

class World: public Object {
    Q_OBJECT
    Q_PROPERTY(Camera* camera READ camera FINAL)
    Q_PROPERTY(AmbientLight* ambientLight READ ambientLight FINAL)
    Q_PROPERTY(Dynamic* dynamic READ dynamic FINAL)
    Q_PROPERTY(QQmlListProperty<Entity> entities READ qmlEntities)
    Q_CLASSINFO("DefaultProperty", "entities")

public:
    World(QObject* parent = nullptr);
    ~World() override;

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

    friend QDataStream& operator<<(QDataStream& s, const World& v);
    friend QDataStream& operator>>(QDataStream& s, World& v);

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

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_H
