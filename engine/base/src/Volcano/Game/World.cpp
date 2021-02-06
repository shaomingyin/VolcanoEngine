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

VOLCANO_GAME_END
