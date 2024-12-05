//
//
#ifndef VOLCANO_GRAPHICS_CANVAS_H
#define VOLCANO_GRAPHICS_CANVAS_H

#include <vector>

#include <nanovg.h>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/FrameBuffer.h>

VOLCANO_GRAPHICS_BEGIN

class Canvas: public FrameBuffer {
public:
	Canvas(int w, int h);
	virtual ~Canvas() = default;

public:
	void render() const {
		if (makeCurrent()) {
			nvg::beginFrame(width(), height(), 1.0f);
			paint();
			nvg::endFrame();
		}
	}

protected:
	virtual void paint() const;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CANVAS_H
