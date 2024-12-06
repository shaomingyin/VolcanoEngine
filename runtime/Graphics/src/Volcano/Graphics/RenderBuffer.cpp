//
//
#include <Volcano/Graphics/RenderBuffer.h>

VOLCANO_GRAPHICS_BEGIN

RenderBuffer::RenderBuffer(int w, int h, int flags)
	: Target(w, h)
	, flags_(flags & (FlagAttachDepth | FlagAttachStencil)) {
	if (flags_ == 0) {
		throw std::runtime_error("Invalid flags.");
	}
}

RenderBuffer::~RenderBuffer() {
}

void RenderBuffer::resize(int w, int h) {
	Target::resize(w, h);
	// TODO
}

bool RenderBuffer::makeCurrent() {
	return true;
}

VOLCANO_GRAPHICS_END
