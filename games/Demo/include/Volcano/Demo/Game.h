//
//
#ifndef VOLCANO_DEMO_GAME_H
#define VOLCANO_DEMO_GAME_H

#include <Volcano/World/Scene.h>
#include <Volcano/Physics/System.h>
#include <Volcano/Demo/Common.h>

VOLCANO_DEMO_BEGIN

class Game: public World::Scene {
public:
    Game();
    ~Game() override;

public:
    const std::string& name() const noexcept override;
    entt::entity mainCamera() const noexcept override;
    void update(Clock::duration elapsed) noexcept override;

private:
    Physics::System physics_system_;
    entt::entity player_;
};

VOLCANO_DEMO_END

#endif // VOLCANO_DEMO_GAME_H
