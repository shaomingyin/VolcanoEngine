//
//
#ifndef VOLCANO_GRAPHICS_VIEW_H
#define VOLCANO_GRAPHICS_VIEW_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Camera.h>

VOLCANO_GRAPHICS_BEGIN

class View {
public:
	View();
	virtual ~View();

public:
	Eigen::Matrix4f& viewMatrix() {
		return view_matrix_;
	}

	const Eigen::Matrix4f& viewMatrix() const {
		return view_matrix_;
	}

	Eigen::Matrix4f& projectionMatrix() {
		projection_matrix_;
	}

	const Eigen::Matrix4f& projectionMatrix() const {
		projection_matrix_;
	}

private:
	Eigen::Matrix4f view_matrix_;
	Eigen::Matrix4f projection_matrix_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VIEW_H
