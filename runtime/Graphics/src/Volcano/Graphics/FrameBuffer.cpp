//
//
#include <Volcano/Error.h>
#include <Volcano/Graphics/FrameBuffer.h>

VOLCANO_GRAPHICS_BEGIN

FrameBuffer::FrameBuffer(int w, int h, int flags)
	: Target(w, h)
	, flags_(flags & (FlagAttachTexture | FlagAttachDepth | FlagAttachStencil)) {
	if (flags_ == 0) {
		throw Error(Errc::InvalidParameter);
	}
	glGenFramebuffers(1, &id_);
	if (id_ == 0) {
		throw Error(Errc::OutOfResource);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, id_);
	if (flags_ & FlagAttachTexture) {
		glGenTextures(1, &texture_);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_, 0);
	}
	if (flags_ & FlagAttachDepth) {
	}
	if (flags_ & FlagAttachStencil) {
	}
}

FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &id_);
}

void FrameBuffer::resize(int w, int h) {
	Target::resize(w, h);
	// TODO
}

VOLCANO_GRAPHICS_END
