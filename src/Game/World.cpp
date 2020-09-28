//
//
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

World::World(QObject *parent):
    QObject(parent),
    m_camera(nullptr)
{
}

World::~World(void)
{
}

void World::update(float elapsed)
{
}

QQmlListProperty<Scene> World::scenes(void)
{
    return QQmlListProperty<Volcano::Game::Scene>(this, this,
        &World::appendScene,
        &World::sceneCount,
        &World::sceneAt,
        &World::clearScene,
        &World::replaceScene,
        &World::removeLastScene);
}

QQmlListProperty<Camera> World::cameras(void)
{
    return QQmlListProperty<Volcano::Game::Camera>(this, this,
        &World::appendCamera,
        &World::cameraCount,
        &World::cameraAt,
        &World::clearCamera,
        &World::replaceCamera,
        &World::removeLastCamera);
}

void World::appendScene(QQmlListProperty<Volcano::Game::Scene> *p, Scene *n)
{
    reinterpret_cast<World *>(p->data)->m_sceneList.append(n);
}

int World::sceneCount(QQmlListProperty<Volcano::Game::Scene> *p)
{
    return reinterpret_cast<World *>(p->data)->m_sceneList.count();
}

Scene *World::sceneAt(QQmlListProperty<Volcano::Game::Scene> *p, int i)
{
    return reinterpret_cast<World *>(p->data)->m_sceneList.at(i);
}

void World::clearScene(QQmlListProperty<Volcano::Game::Scene> *p)
{
    reinterpret_cast<World *>(p->data)->m_sceneList.clear();
}

void World::replaceScene(QQmlListProperty<Volcano::Game::Scene> *p, int i, Scene *n)
{
    reinterpret_cast<World *>(p->data)->m_sceneList.replace(i, n);
}

void World::removeLastScene(QQmlListProperty<Volcano::Game::Scene> *p)
{
    reinterpret_cast<World *>(p->data)->m_sceneList.removeLast();
}

void World::appendCamera(QQmlListProperty<Volcano::Game::Camera> *p, Camera *n)
{
    reinterpret_cast<World *>(p->data)->m_cameraList.append(n);
}

int World::cameraCount(QQmlListProperty<Volcano::Game::Camera> *p)
{
    return reinterpret_cast<World *>(p->data)->m_cameraList.count();
}

Camera *World::cameraAt(QQmlListProperty<Volcano::Game::Camera> *p, int i)
{
    return reinterpret_cast<World *>(p->data)->m_cameraList.at(i);
}

void World::clearCamera(QQmlListProperty<Volcano::Game::Camera> *p)
{
    reinterpret_cast<World *>(p->data)->m_cameraList.clear();
}

void World::replaceCamera(QQmlListProperty<Volcano::Game::Camera> *p, int i, Camera *n)
{
    reinterpret_cast<World *>(p->data)->m_cameraList.replace(i, n);
}

void World::removeLastCamera(QQmlListProperty<Volcano::Game::Camera> *p)
{
    reinterpret_cast<World *>(p->data)->m_cameraList.removeLast();
}

VOLCANO_GAME_END
