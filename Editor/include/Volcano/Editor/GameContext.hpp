//
//
#ifndef VOLCANO_EDITOR_GAMECONTEXT_HPP
#define VOLCANO_EDITOR_GAMECONTEXT_HPP

#include <Volcano/Game/Context.hpp>
#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class GameContext: public Game::Context {
    Q_OBJECT

public:
    GameContext(QObject *parent = nullptr);
    ~GameContext(void) override;

public:
    Duration elapsed(void) override;
    Input::Service *inputService(void) override;
    Graphics::Service *graphicsService(void) override;
    Physics::Service *physicsService(void) override;
    Sound::Service *soundService(void) override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_GAMECONTEXT_HPP
