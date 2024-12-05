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
	glGenFramebuffers(1, &id_);
	if (id_ == 0) {
		throw std::runtime_error("Failed to create frame buffer.");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, id_);
	//glFramebufferParameteri(GL_FRAMEBUFFER, )
	if (flags & FlagAttachColor) {

	}
	if (flags & FlagAttachDepth) {

	}
	if (flags & FlagAttachStencil) {

	}
}

FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &id_);
}

void FrameBuffer::resize(int w, int h) {
	Target::resize(w, h);
	// TODO
}

bool FrameBuffer::makeCurrent() const {
	glBindFramebuffer(GL_FRAMEBUFFER, id_);
	return true;
}

VOLCANO_GRAPHICS_END
