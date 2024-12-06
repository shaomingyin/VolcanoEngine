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
	bool makeCurrent() override;

	int flags() const {
		return flags_;
	}

private:
	int flags_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERBUFFER_H
