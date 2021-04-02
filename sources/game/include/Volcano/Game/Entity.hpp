//
//
#ifndef VOLCANO_GAME_ENTITY_HPP
#define VOLCANO_GAME_ENTITY_HPP

#include <vector>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

class Entity: public Object {
    sigslot::signal<const Eigen::Vector3f &> setPosition;
    sigslot::signal<const Eigen::Vector3f &> setScale;
    sigslot::signal<const Eigen::Quaternionf &> setRotation;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ENTITY_HPP
