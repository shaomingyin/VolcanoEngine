//
//
#ifndef VOLCANO_GRAPHICS_FRAMEBUFFER_H
#define VOLCANO_GRAPHICS_FRAMEBUFFER_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Target.h>

VOLCANO_GRAPHICS_BEGIN

class FrameBuffer: public Target {
public:
	enum {
		FlagAttachColor = 0x1,
		FlagAttachDepth = 0x2,
		FlagAttachStencil = 0x4
	};

public:
	FrameBuffer(int w, int h, int flags = FlagAttachColor);
	~FrameBuffer() override;

public:
	void resize(int w, int h) override;
	bool makeCurrent() const override;

	int flags() const {
		return flags_;
	}

private:
	int flags_;
	GLuint id_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_FRAMEBUFFER_H
