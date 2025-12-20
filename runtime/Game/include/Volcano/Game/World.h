//
//
#ifndef VOLCANO_GAME_WORLD_H
#define VOLCANO_GAME_WORLD_H

#include <QSet>
#include <QList>
#include <QObject>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Scene.h>
#include <Volcano/Game/Dynamic.h>
#include <Volcano/Game/Camera.h>
#include <Volcano/Game/Light.h>
#include <Volcano/Game/Object.h>

VOLCANO_GAME_BEGIN

class World: public Object {
    Q_OBJECT
    Q_PROPERTY(Camera* camera READ camera FINAL)
    Q_PROPERTY(Light* ambientLight READ ambientLight FINAL)
    Q_PROPERTY(Dynamic* dynamic READ dynamic FINAL)
    Q_PROPERTY(QQmlListProperty<Scene> scenes READ qmlScenes)
    Q_CLASSINFO("DefaultProperty", "scenes")

public:
    World(QObject* parent = nullptr);
    ~World() override;

public:
    Camera* camera() {
        return &camera_;
    }

    Light* ambientLight() {
        return &ambient_light_;
    }

    Dynamic* dynamic() {
        return &dynamic_;
    }

    const QList<Scene*>& scenes() const {
        return scenes_;
    }

    void update(Clock::duration elapsed);
    void appendScene(Scene* p);
    Scene* sceneAt(qsizetype i);
    void clearScenes();
    qsizetype sceneCount();
    void removeLastScene();
    void replaceScene(qsizetype i, Scene* p);
    QQmlListProperty<Scene> qmlScenes();

signals:
    void sceneAdded(Scene* scene);
    void sceneRemoved(Scene * scene);
    void componentAdded(Scene* scene, Entity* entity, Component *component);
    void componentRemoved(Scene* scene, Entity* entity, Component *component);

private:
    void onSceneAdded(Scene* scene);
    void onComponentAdded(Scene* scene, Entity* entity, Component* component);
    void onSceneRemoved(Scene* scene);
    void onComponentRemoved(Scene* scene, Entity* entity, Component* component);

private:
    Camera camera_;
    Light ambient_light_;
    Dynamic dynamic_;
    QList<Scene*> scenes_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_H
