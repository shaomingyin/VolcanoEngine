//
//
#include <Volcano/Graphics/Canvas.h>

VOLCANO_GRAPHICS_BEGIN

Canvas::Canvas(int w, int h)
	: FrameBuffer(w, h) {
}

void Canvas::paint() const {
	nvg::beginPath();
	nvg::rect(0, 0, width(), height());
	nvg::strokeColor(nvg::RGBf(0.0f, 0.0f, 0.0f));
	nvg::stroke();
}

VOLCANO_GRAPHICS_END
