//
//
#ifndef VOLCANO_WORLD_LISTENER_H
#define VOLCANO_WORLD_LISTENER_H

#include <QObject>

#include <Volcano/World/Common.h>
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

class Listener: public QObject {
    Q_OBJECT

public:
    Listener(Scene& scene, QObject* parent = nullptr);

public:
    Scene& scene() {
        return scene_;
    }

    const Scene& scene() const {
        return scene_;
    }

protected slots:
    virtual void onEntityAdded(Entity* entity);
    virtual void onEntityRemoved(Entity* entity);
    virtual void onComponentAdded(Entity* entity, Component* component);
    virtual void onComponentRemoved(Entity* entity, Component* component);

private:
    using EntitySignalConnectons = QMultiHash<Entity*, QMetaObject::Connection>;

private:
    Scene& scene_;
    EntitySignalConnectons entity_signal_connections_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_LISTENER_H
