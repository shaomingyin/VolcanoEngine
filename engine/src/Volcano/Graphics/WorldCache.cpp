//
//
#include <Volcano/Graphics/WorldCache.hpp>

VOLCANO_GRAPHICS_BEGIN

WorldCache::WorldCache(QObject *parent):
    QObject(parent),
    m_world(nullptr)
{
}

WorldCache::~WorldCache(void)
{
}

World *WorldCache::world(void)
{
    return m_world;
}

void WorldCache::setWorld(World *world)
{
    if (m_world != world) {
        m_world = world;
        worldChanged(world);
    }
}

void WorldCache::update(const QVector3D &position)
{

}

VOLCANO_GRAPHICS_END
