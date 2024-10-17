//
//
#ifndef VOLCANO_GAME_ACTOR_H
#define VOLCANO_GAME_ACTOR_H

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Object.h>

VOLCANO_GAME_BEGIN

class Actor: public Object {
public:
	Actor(Context& context);
	~Actor() override;

public:
	Eigen::Transform3f& transform() {
		return transform_;
	}

	const Eigen::Transform3f& transform() const {
		return transform_;
	}

private:
	Eigen::Transform3f transform_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ACTOR_H
