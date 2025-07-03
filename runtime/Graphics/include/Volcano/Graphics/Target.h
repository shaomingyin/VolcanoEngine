//
//
#ifndef VOLCANO_GRAPHICS_TARGET_H
#define VOLCANO_GRAPHICS_TARGET_H

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Target {
public:
	Target(int width, int height)
		: width_(width)
		, height_(height) {
	}

	virtual ~Target() = default;

public:
	bool isEnabled() const noexcept {
		return false;// FIXME
	}

	void enable() noexcept {
	}

	void disable() noexcept {
	}

	int width() const noexcept {
		return width_;
	}

	int height() const noexcept {
		return height_;
	}

	virtual void resize(int width, int height);

private:
	int width_;
	int height_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_TARGET_H
