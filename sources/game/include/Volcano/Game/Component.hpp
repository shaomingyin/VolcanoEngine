//
//
#ifndef VOLCANO_GAME_COMPONENT_HPP
#define VOLCANO_GAME_COMPONENT_HPP

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Component {
    RTTR_ENABLE()

public:
    Component(void);
    virtual ~Component(void);
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_COMPONENT_HPP
