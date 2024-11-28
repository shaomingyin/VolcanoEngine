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

bool Target::beginFrame() {
	return true;
}

void Target::endFrame() {
}

VOLCANO_GRAPHICS_END
