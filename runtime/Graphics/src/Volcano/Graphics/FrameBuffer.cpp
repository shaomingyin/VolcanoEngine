//
//
#include <Volcano/Graphics/FrameBuffer.h>

VOLCANO_GRAPHICS_BEGIN

FrameBuffer::FrameBuffer(int w, int h, int flags)
	: Target(w, h)
	, flags_(flags & (FlagAttachColor | FlagAttachDepth | FlagAttachStencil)) {
	if (flags_ == 0) {
		throw std::runtime_error("Invalid flags.");
	}
}

FrameBuffer::~FrameBuffer() {
}

void FrameBuffer::resize(int w, int h) {
	Target::resize(w, h);
	// TODO
}

VOLCANO_GRAPHICS_END
