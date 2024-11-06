//
//
#ifndef VOLCANO_GRAHPICS_LIGHT_H
#define VOLCANO_GRAHPICS_LIGHT_H

#include <Volcano/Color.h>
#include <Volcano/World/Component.h>
#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Light: public World::Movable {
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

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAHPICS_LIGHT_H
