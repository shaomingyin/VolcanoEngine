//
//
#ifndef VOLCANO_GRAPHICS_PASS_H
#define VOLCANO_GRAPHICS_PASS_H

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Frame;

class Pass {
public:
	Pass() = default;
	virtual ~Pass() = default;

public:
	virtual void resize(int width, int height) noexcept = 0;
	virtual void execute(const Frame& frame) = 0;

private:
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_PASS_H
