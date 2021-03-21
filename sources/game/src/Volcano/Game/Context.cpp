//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Game/Context.hpp>

VOLCANO_GAME_BEGIN

Context::Context(void)
{
}

Context::~Context(void)
{
}

bool Context::init(void)
{
    return true;
}

void Context::update(float elapsed)
{
}

World &Context::world(void)
{
    return m_world;
}

VOLCANO_GAME_END
