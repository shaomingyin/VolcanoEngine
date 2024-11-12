//
//
#ifndef VOLCANO_GRAPHICS_CANVAS_H
#define VOLCANO_GRAPHICS_CANVAS_H

#include <vector>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/FrameBuffer.h>

VOLCANO_GRAPHICS_BEGIN

class Canvas: public FrameBuffer {
public:
	Canvas(int w, int h);
	virtual ~Canvas() = default;

public:
	void resize(int w, int h) override;
	bool beginFrame() override;
	void endFrame() override;

public:
	void reset() {
		transform_.setIdentity();
	}

	void moveTo(float dx, float dy) {
		transform_.translate(Eigen::Vector2f(dx, dy));
	}

	void line(float x, float y) {
	}

private:
	Eigen::Affine2f transform_;
	//std::vector<std::pair<Eigen::Affine2f>> ops_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CANVAS_H
