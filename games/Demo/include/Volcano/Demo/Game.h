//
//
#ifndef VOLCANO_DEMO_GAME_H
#define VOLCANO_DEMO_GAME_H

#include <memory>

#include <async++.h>

#include <Volcano/World/Scene.h>
#include <Volcano/Physics/System.h>
#include <Volcano/Demo/Common.h>

VOLCANO_DEMO_BEGIN

class Game: public World::Scene {
public:
    Game(const nlohmann::json& metadata);
    ~Game() override;

public:
    void update(Clock::duration elapsed) noexcept override;
    entt::entity player() const noexcept override;

private:
    Physics::System physics_;
};

VOLCANO_DEMO_END

#endif // VOLCANO_DEMO_GAME_H
