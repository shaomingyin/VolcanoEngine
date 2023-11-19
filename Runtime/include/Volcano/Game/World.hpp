//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <QList>
#include <QQmlListProperty>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Camera.hpp>
#include <Volcano/Game/Scene.hpp>
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

using SceneList = QList<Scene *>;

class World: public Object {
    Q_OBJECT
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(QQmlListProperty<Scene> scenes READ qmlSceneList)
    Q_CLASSINFO("DefaultProperty", "scenes")

public:
    World(QObject *parent = nullptr);
    ~World(void) override;

public:
    Camera* camera();
    void setCamera(Camera* p);
    const SceneList &sceneList(void) const;
    void appendScene(Scene *scene);
    qsizetype sceneCount(void) const;
    Scene *sceneAt(qsizetype index);
    void clearScenes(void);
    void replaceScene(qsizetype index, Scene *scene);
    void removeLastScene(void);

public:
    QQmlListProperty<Scene> qmlSceneList() {
        return { this, this,
            [](QQmlListProperty<Scene>* property, Scene* p) { reinterpret_cast<World*>(property->data)->appendScene(p); },
            [](QQmlListProperty<Scene>* property) { return reinterpret_cast<World*>(property->data)->sceneCount(); },
            [](QQmlListProperty<Scene>* property, qsizetype index) { return reinterpret_cast<World*>(property->data)->sceneAt(index); },
            [](QQmlListProperty<Scene>* property) { reinterpret_cast<World*>(property->data)->clearScenes(); },
            [](QQmlListProperty<Scene>* property, qsizetype index, Scene* p) { reinterpret_cast<World*>(property->data)->replaceScene(index, p); },
            [](QQmlListProperty<Scene>* property) { reinterpret_cast<World*>(property->data)->removeLastScene(); }
        };
    }

signals:
    void cameraChanged(Camera* p);
    void sceneAdded(Scene* p);
    void sceneRemoved(Scene* p);

private:
    Camera* camera_;
    SceneList sceneList_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
