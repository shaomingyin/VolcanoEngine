//
//
#ifndef VOLCANO_WORLD_ACTOR_H
#define VOLCANO_WORLD_ACTOR_H

#include <sigslot/signal.hpp>

#include <Volcano/Math.h>
#include <Volcano/WORLD/Common.h>
#include <Volcano/WORLD/Object.h>

VOLCANO_WORLD_BEGIN

class Actor: public Object {
public:
	sigslot::signal_st<const Transform&> setTransform;
	sigslot::signal_st<const Eigen::Vector3f&> setTranslation;
	sigslot::signal_st<const Eigen::Vector3f&> setScale;
	sigslot::signal_st<const Eigen::Quaternionf&> setRotation;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_ACTOR_H
