//
//
#ifndef VOLCANO_GRAPHICS_TARGET_H
#define VOLCANO_GRAPHICS_TARGET_H

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Target {
public:
	virtual ~Target() = default;

public:
	bool isEnabled() const {
		return enabled_;
	}

	void enable() {
		enabled_ = true;
	}

	void disable() {
		enabled_ = false;
	}

	int width() const {
		return width_;
	}

	int height() const {
		return height_;
	}

	virtual void resize(int widht, int height);

protected:
	Target(int width, int height);

private:
	bool enabled_;
	int width_;
	int height_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_TARGET_H
