//
//
#include <Volcano/VM/World.hpp>

VOLCANO_VM_BEGIN

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

void World::applyToView(Graphics::View &view)
{
}

VOLCANO_VM_END
