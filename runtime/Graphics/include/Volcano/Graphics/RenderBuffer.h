//
//
#ifndef VOLCANO_GRAPHICS_RENDERBUFFER_H
#define VOLCANO_GRAPHICS_RENDERBUFFER_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Target.h>

VOLCANO_GRAPHICS_BEGIN

class RenderBuffer: public Target {
public:
	enum {
		FlagAttachDepth = 0x1,
		FlagAttachStencil = 0x2
	};

public:
	RenderBuffer(int w, int h, int flags = FlagAttachDepth | FlagAttachStencil);
	~RenderBuffer() override;

public:
	void resize(int w, int h) override;
	bool beginFrame() override;
	void endFrame() override;

	int flags() const {
		return flags_;
	}

	GLuint id() const {
		return id_;
	}

	void bind(GLenum target = GL_RENDERBUFFER) const {
		glBindRenderbuffer(target, id_);
	}

private:
	int flags_;
	GLuint id_;
	GLuint texture_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERBUFFER_H
