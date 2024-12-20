//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <QList>
#include <QMultiMap>
#include <QObject>
#include <QQmlListProperty>

#include <Volcano/World/Common.h>
#include <Volcano/World/Entity.h>

VOLCANO_WORLD_BEGIN

class Scene: public QObject {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Entity> entities READ qmlEntities)
    Q_CLASSINFO("DefaultProperty", "entities")

public:
    Scene(QObject* parent = nullptr);

public:
    Entity& global() {
        return global_;
    }

    const Entity& global() const {
        return global_;
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
	virtual void update(Duration elapsed);

signals:
    void entityAdded(Entity* entity);
    void entityRemoved(Entity* entity);

private:
    QList<Entity*> entities_;
    Entity global_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCENE_H
