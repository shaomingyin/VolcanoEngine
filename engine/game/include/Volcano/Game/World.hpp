//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <QList>
#include <QString>
#include <QSharedPointer>
#include <QObject>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Scene.hpp>
#include <Volcano/Game/Camera.hpp>

VOLCANO_GAME_BEGIN

using CameraPtr = QSharedPointer<Camera>;
using CameraPtrList = QList<CameraPtr>;

using ScenePtr = QSharedPointer<Scene>;
using ScenePtrList = QList<ScenePtr>;

class VOLCANO_GAME_API World: public QObject {
    Q_OBJECT

public:
    World(QObject *parent = nullptr);
    ~World(void) override;

public:
    void update(float elapsed);
    void addCamera(CameraPtr camera);
    void removeCamera(CameraPtr camera);
    const CameraPtrList cameras(void) const;
    void addScene(ScenePtr scene);
    void removeScene(ScenePtr scene);
    const ScenePtrList scenes(void) const;

signals:
    void cameraAdded(CameraPtr camera);
    void cameraRemoved(CameraPtr camera);
    void sceneAdded(ScenePtr scene);
    void sceneRemoved(ScenePtr scene);

private:
    CameraPtrList m_cameras;
    ScenePtrList m_scenes;
};

VOLCANO_INLINE const CameraPtrList World::cameras(void) const
{
    return m_cameras;
}

VOLCANO_INLINE const ScenePtrList World::scenes(void) const
{
    return m_scenes;
}

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
