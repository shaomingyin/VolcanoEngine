//
//
#ifndef VOLCANO_GAME_LIGHT_H
#define VOLCANO_GAME_LIGHT_H

#include <Volcano/Color.h>
#include <Volcano/Game/Common.h>
#include <Volcano/Game/Actor.h>

VOLCANO_GAME_BEGIN

class Light: public Actor {
public:
	Light(Context& context);
	~Light() override;

public:
	const Color& color() const {
		return color_;
	}

	void setColor(const Color& v) {
		color_ = v;
	}

private:
	Color color_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_LIGHT_H
