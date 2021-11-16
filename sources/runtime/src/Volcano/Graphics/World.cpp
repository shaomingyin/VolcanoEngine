//
//
#include <Volcano/Graphics/World.hpp>

VOLCANO_GRAPHICS_BEGIN

World::World(QObject *parent):
    QObject(parent)
{
}

World::~World(void)
{
}

void World::buildDrawingSet(DrawingSet &out, const Camera &cam)
{
}

VOLCANO_GRAPHICS_END
