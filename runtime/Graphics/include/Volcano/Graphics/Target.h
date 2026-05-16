//
//
#ifndef VOLCANO_GRAPHICS_TARGET_H
#define VOLCANO_GRAPHICS_TARGET_H

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Target {
public:
	Target(uint32_t width, uint32_t height);
	virtual ~Target() = default;

public:
	uint32_t width() const noexcept {
		return width_;
	}

	uint32_t height() const noexcept {
		return height_;
	}

private:
	uint32_t width_;
	uint32_t height_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_TARGET_H
