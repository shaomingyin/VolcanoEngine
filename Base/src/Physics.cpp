//
//
#include <Volcano/Physics.h>

VOLCANO_BEGIN

Physics::Physics(Scene *scene):
    Scene::Listener(scene)
{
}

Physics::~Physics(void)
{
}

void Physics::update(float elapsed)
{
}

VOLCANO_END
