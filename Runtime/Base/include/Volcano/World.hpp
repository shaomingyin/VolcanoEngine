//
//
#ifndef VOLCANO_WORLD_HPP
#define VOLCANO_WORLD_HPP

#include <QList>
#include <QQmlListProperty>

#include <Volcano/Common.hpp>
#include <Volcano/Camera.hpp>
#include <Volcano/Scene.hpp>
#include <Volcano/Object.hpp>

VOLCANO_BEGIN

using SceneList = QList<Scene *>;

class World: public Object {
    Q_OBJECT
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(QQmlListProperty<Scene> scenes READ qmlSceneList)
    Q_CLASSINFO("DefaultProperty", "scenes")

public:
    World(QObject *parent = nullptr);
    World(Context& context, QObject *parent = nullptr);

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
    QQmlListProperty<Scene> qmlSceneList();

signals:
    void cameraChanged(Camera* p);
    void sceneAdded(Scene* p);
    void sceneRemoved(Scene* p);

protected:
    void updateView() const override;
    void updateState(Duration elapsed) override;

private:
    Camera* camera_;
    SceneList sceneList_;
};

VOLCANO_END

#endif // VOLCANO_WORLD_HPP
