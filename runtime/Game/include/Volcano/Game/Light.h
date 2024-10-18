//
//
#ifndef VOLCANO_GAME_LIGHT_H
#define VOLCANO_GAME_LIGHT_H

#include <Volcano/Color.h>
#include <Volcano/Game/Common.h>

VOLCANO_GAME_BEGIN

class Light {
public:
	Light();
	virtual ~Light();

public:
	const Color& color() const {
		return color_;
	}

	void setColor(const Color& v) {
		color_ = v;
	}

	float strength() const {
		return strength_;
	}

	void setStrength(float v) {
		strength_ = std::clamp(v, 0.0f, 1.0f);
	}

private:
	Color color_;
	float strength_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_LIGHT_H
