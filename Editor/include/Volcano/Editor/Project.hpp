//
//
#ifndef VOLCANO_EDITOR_PROJECT_HPP
#define VOLCANO_EDITOR_PROJECT_HPP

#include <memory>
#include <type_traits>

#include <utils/filepath.h>
#include <projectexplorer/project.h>

#include <Volcano/Game/Context.hpp>
#include <Volcano/Game/World.hpp>

#include <Volcano/Editor/Common.hpp>
#include <Volcano/Editor/GameContext.hpp>

VOLCANO_EDITOR_BEGIN

class Project: public ProjectExplorer::Project {
    Q_OBJECT

public:
    Project(const Utils::FilePath &fileName, Game::Context *gameContext);
    ~Project(void) override;

public:
    bool init(void);
    Game::Context *gameContext(void);
    Game::World *gameWorld(void);

    template <typename T>
    T *newGameObject(QObject *parent = nullptr)
    {
        static_assert(std::is_base_of<Game::Object, T>::value);

        Q_ASSERT(m_gameContext != nullptr);

        auto p = std::make_unique<T>(m_gameContext, parent);
        if (!p)
            return nullptr;

        return p.release();
    }

private:
    Game::Context *m_gameContext;
    Game::World *m_gameWorld;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PROJECT_HPP
