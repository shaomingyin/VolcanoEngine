//
//
#include <Volcano/Graphics/Target.h>

VOLCANO_GRAPHICS_BEGIN

Target::Target(int width, int height)
	: width_(width)
	, height_(height) {
}

void Target::resize(int width, int height) {
	width_ = width;
	height_ = height;
}

VOLCANO_GRAPHICS_END
