//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Scene.hpp>

VOLCANO_GAME_BEGIN

class World {
public:
    World();
    virtual ~World();

public:
    virtual void update(Clock::duration elapsed);

private:
    Scene scene_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
