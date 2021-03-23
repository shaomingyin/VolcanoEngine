//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <list>
#include <string>
#include <memory>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Scene.hpp>
#include <Volcano/Game/Camera.hpp>

VOLCANO_GAME_BEGIN

using CameraPtr = std::shared_ptr<Camera>;
using CameraPtrList = std::list<CameraPtr>;

using ScenePtr = std::shared_ptr<Scene>;
using ScenePtrList = std::list<ScenePtr>;

class World {
public:
    World(void);
    virtual ~World(void);

public:
    virtual void update(float elapsed);
    void addCamera(CameraPtr camera);
    void removeCamera(CameraPtr camera);
    const CameraPtrList cameras(void) const;
    void addScene(ScenePtr scene);
    void removeScene(ScenePtr scene);
    const ScenePtrList scenes(void) const;

public:
    sigslot::signal<CameraPtr> cameraAdded;
    sigslot::signal<CameraPtr> cameraRemoved;
    sigslot::signal<ScenePtr> sceneAdded;
    sigslot::signal<ScenePtr> sceneRemoved;

private:
    CameraPtrList m_cameras;
    ScenePtrList m_scenes;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
