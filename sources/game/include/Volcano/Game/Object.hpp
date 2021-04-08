//
//
#ifndef VOLCANO_GAME_OBJECT_HPP
#define VOLCANO_GAME_OBJECT_HPP

#include <string_view>

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Object {
public:
    sigslot::signal<std::string_view> setName;
    sigslot::signal<> enable;
    sigslot::signal<> disable;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_OBJECT_HPP
