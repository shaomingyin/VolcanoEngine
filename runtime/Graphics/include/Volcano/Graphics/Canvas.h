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

	void lineTo(float x, float y) {
	}

private:
	struct Command {
		Eigen::Affine3f transform;
		virtual void render() = 0;
	};

	struct DrawLineCommand: public Command {
		Eigen::Vector3f a;
		Eigen::Vector3f b;
		void render() override;
	};

	Eigen::Affine2f transform_;
	//std::vector<std::pair<Eigen::Affine2f>> commands_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CANVAS_H
