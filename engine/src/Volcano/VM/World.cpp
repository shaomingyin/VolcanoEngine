//
//
#include <Volcano/VM/World.hpp>

VOLCANO_VM_BEGIN

World::World(void):
    m_dynamicWorld(nullptr)
{
}

World::~World(void)
{
    if (m_dynamicWorld != nullptr)
        shutdown();
}

bool World::init(void)
{
    VOLCANO_ASSERT(m_dynamicWorld == nullptr);

    return true;
}

void World::shutdown(void)
{
    delete m_dynamicWorld;
    m_dynamicWorld = nullptr;
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
