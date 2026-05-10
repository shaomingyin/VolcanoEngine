//
//
#ifndef VOLCANO_STAGE_ACTOR_H
#define VOLCANO_STAGE_ACTOR_H

#include <sigslot/signal.hpp>

#include <Volcano/Math.h>
#include <Volcano/STAGE/Common.h>
#include <Volcano/STAGE/Object.h>

VOLCANO_STAGE_BEGIN

class Actor: public Object {
public:
	sigslot::signal_st<const Transform&> setTransform;
	sigslot::signal_st<const Eigen::Vector3f&> setTranslation;
	sigslot::signal_st<const Eigen::Vector3f&> setScale;
	sigslot::signal_st<const Eigen::Quaternionf&> setRotation;
};

VOLCANO_STAGE_END

#endif // VOLCANO_STAGE_ACTOR_H
