//
//
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

World::World(QObject *parent):
    QObject(parent)
{
}

World::~World(void)
{
}

void World::update(float elapsed)
{

}

void World::addCamera(CameraPtr camera)
{
    qsizetype i = m_cameras.indexOf(camera);
    if (i < 0) {
        m_cameras.append(camera);
        cameraAdded(camera);
    }
}

void World::removeCamera(CameraPtr camera)
{
    qsizetype i = m_cameras.indexOf(camera);
    if (i >= 0) {
        m_cameras.removeAt(i);
        cameraRemoved(camera);
    }
}

void World::addScene(ScenePtr scene)
{
    qsizetype i = m_scenes.indexOf(scene);
    if (i < 0) {
        m_scenes.append(scene);
        sceneAdded(scene);
    }
}

void World::removeScene(ScenePtr scene)
{
    qsizetype i = m_scenes.indexOf(scene);
    if (i < 0) {
        m_scenes.removeAt(i);
        sceneRemoved(scene);
    }
}

VOLCANO_GAME_END
