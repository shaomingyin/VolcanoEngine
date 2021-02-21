//
//
#include <Volcano/Game/Scene.hpp>

VOLCANO_GAME_BEGIN

Scene::Scene(QObject *parent):
    QObject(parent)
{
}

Scene::~Scene(void)
{
}

bool Scene::init(void)
{
    return true;
}

void Scene::shutdown(void)
{
}

void Scene::update(float elapsed)
{
}

VOLCANO_GAME_END
