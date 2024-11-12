//
//
#include <Volcano/Error.h>
#include <Volcano/Graphics/Canvas.h>

VOLCANO_GRAPHICS_BEGIN

Canvas::Canvas(int w, int h)
	: FrameBuffer(w, h) {
}

void Canvas::resize(int w, int h) {
	FrameBuffer::resize(w, h);
}

bool Canvas::beginFrame() {
	if (!FrameBuffer::beginFrame()) {
		return false;
	}

	return true;
}

void Canvas::endFrame() {
	FrameBuffer::endFrame();
}

VOLCANO_GRAPHICS_END
