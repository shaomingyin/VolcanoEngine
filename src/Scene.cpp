//
//
#include <Volcano/Scene.h>

VOLCANO_BEGIN

Scene::Scene(QObject *parent):
    Object(parent)
{
    qDebug() << "scene constructed";
}

Scene::~Scene(void)
{
}

void Scene::update(float elapsed)
{

}

VOLCANO_END
