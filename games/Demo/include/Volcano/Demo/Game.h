//
//
#ifndef VOLCANO_DEMO_GAME_H
#define VOLCANO_DEMO_GAME_H

#include <rttr/type>

#include <Volcano/World/Scene.h>
#include <Volcano/Demo/Common.h>

VOLCANO_DEMO_BEGIN

class Game : public World::Scene {
    RTTR_ENABLE(World::Scene)

public:
    Game();
    ~Game() override;

public:
    entt::entity mainCamera() const noexcept override;
    void update(World::Clock::duration elapsed) noexcept override;

private:
    entt::entity main_camera_;
};

VOLCANO_DEMO_END

#endif // VOLCANO_DEMO_GAME_H
