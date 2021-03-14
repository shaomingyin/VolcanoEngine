//
//
#include <filesystem>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Game/Context.hpp>

VOLCANO_GAME_BEGIN

Context::Context(void)
{
}

Context::~Context(void)
{
}

bool Context::init(std::string_view rootDir)
{
    if (!std::filesystem::is_directory(rootDir))
        return false;

    m_rootDir = rootDir;

    return true;
}

void Context::update(float elapsed)
{
}

const std::string &Context::rootDir(void) const
{
    return m_rootDir;
}

World &Context::world(void)
{
    return m_world;
}

VOLCANO_GAME_END
