//
//
#include <rttr/registration>

#include <Volcano/VM/Scene/World.hpp>

VOLCANO_VM_SCENE_BEGIN

RTTR_REGISTRATION
{
    rttr::registration::class_<World>("World").
        method("test", &World::test);
}

World::World(void)
{
}

World::~World(void)
{
}

bool World::init(void)
{
    return true;
}

void World::shutdown(void)
{
}

void World::update(float elapsed)
{
}

void World::handleEvent(const SDL_Event &evt)
{
}

void World::test(int i)
{
    VOLCANO_LOGI(">> i%d", i);
}

VOLCANO_VM_SCENE_END
