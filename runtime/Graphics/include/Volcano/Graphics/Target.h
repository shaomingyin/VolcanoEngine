//
//
#ifndef VOLCANO_GRAPHICS_TARGET_H
#define VOLCANO_GRAPHICS_TARGET_H

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Target {
public:
	Target(int width, int height);
	virtual ~Target() = default;

public:
	int width() const {
		return width_;
	}

	int height() const {
		return height_;
	}

	virtual void resize(int width, int height);
	virtual bool beginFrame() = 0;
	virtual void endFrame() = 0;

private:
	int width_;
	int height_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_TARGET_H
