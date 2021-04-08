//
//
#ifndef VOLCANO_GAME_SCREEN_HPP
#define VOLCANO_GAME_SCREEN_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Entity.hpp>

VOLCANO_GAME_BEGIN

class Screen: public Entity {
public:
    Screen(void);
    virtual ~Screen(void);
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SCREEN_HPP
