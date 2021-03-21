//
//
#ifndef VOLCANO_GAME_CONTEXTIMPL_HPP
#define VOLCANO_GAME_CONTEXTIMPL_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

class Context {
public:
    Context(void);
    virtual ~Context(void);

public:
    bool init(void);
    void update(float elapsed);
    World &world(void);

private:
    World m_world;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CONTEXTIMPL_HPP
