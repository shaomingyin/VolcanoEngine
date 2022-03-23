//
//
#include <memory>

#include <Volcano/Editor/Project.hpp>

VOLCANO_EDITOR_BEGIN

Project::Project(const Utils::FilePath &fileName, Game::Context *gameContext):
    ProjectExplorer::Project("vep", fileName),
    m_gameContext(gameContext),
    m_gameWorld(nullptr)
{
    Q_ASSERT(m_gameContext != nullptr);
}

Project::~Project(void)
{
}

bool Project::init(void)
{
    Q_ASSERT(m_gameContext != nullptr);
    Q_ASSERT(m_gameWorld == nullptr);

    auto gameWorld = std::make_unique<Game::World>(m_gameContext, this);
    if (!gameWorld)
        return false;

    m_gameWorld = gameWorld.release();

    return true;
}

Game::Context *Project::gameContext(void)
{
    return m_gameContext;
}

Game::World *Project::gameWorld(void)
{
    return m_gameWorld;
}

VOLCANO_EDITOR_END
