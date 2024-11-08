//
//
#include <Volcano/Error.h>
#include <Volcano/Graphics/RenderBuffer.h>

VOLCANO_GRAPHICS_BEGIN

RenderBuffer::RenderBuffer(int w, int h, int flags)
	: Target(w, h)
	, flags_(flags & (FlagAttachDepth | FlagAttachStencil)) {
	if (flags_ == 0) {
		throw Error(Errc::InvalidParameter);
	}
	glGenRenderbuffers(1, &id_);
	if (id_ == 0) {
		throw Error(Errc::OutOfResource);
	}
	glBindRenderbuffer(GL_RENDERBUFFER, id_);
	if (flags_ & FlagAttachDepth) {
	}
	if (flags_ & FlagAttachStencil) {
	}
}

RenderBuffer::~RenderBuffer() {
	glDeleteRenderbuffers(1, &id_);
}

void RenderBuffer::resize(int w, int h) {
	Target::resize(w, h);
	// TODO
}

bool RenderBuffer::beginFrame() {
	glBindRenderbuffer(GL_RENDERBUFFER, id_);
	return true;
}

void RenderBuffer::endFrame() {
}

VOLCANO_GRAPHICS_END
