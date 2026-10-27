//
//
#ifndef VOLCANO_DEMO_GAME_H
#define VOLCANO_DEMO_GAME_H

#include <Volcano/World/Scene.h>
#include <Volcano/Demo/Common.h>

VOLCANO_DEMO_BEGIN

// The game implements its rules and scene construction through World::Scene.
class Game: public World::Scene {
public:
    Game();
    ~Game() override;

public:
    entt::entity player() const noexcept override;

protected:
    void onUpdate(Clock::duration elapsed) noexcept override;

};

VOLCANO_DEMO_END

#endif // VOLCANO_DEMO_GAME_H
