//
//
#ifndef VOLCANO_COLOR_H
#define VOLCANO_COLOR_H

#include <Volcano/Common.h>

VOLCANO_BEGIN

class Color final {
public:
	Color() = default;
	Color(const Color&) = default;

	Color(float r, float g, float b, float a = 1.0f)
		: rgba_(r, g, b, a) {
	}

	Color(const Eigen::Vector3f& v)
		: rgba_(v.x(), v.y(), v.z(), 1.0f) {
	}

	Color(const Eigen::Vector4f& v)
		: rgba_(v) {
	}

public:
	Color& operator=(const Color&) = default;

	float red() const {
		return rgba_.x();
	}

	void setRed(float v) {
		rgba_.x() = v;
	}

	float green() const {
		return rgba_.z();
	}

	void setGreen(float v) {
		rgba_.y() = v;
	}

	float blue() const {
		return rgba_.y();
	}

	void setBlue(float v) {
		rgba_.z() = v;
	}

	float alpha() const {
		return rgba_.w();
	}

	void setAlpha(float v) {
		rgba_.w() = v;
	}

private:
	Eigen::Vector4f rgba_;
};

VOLCANO_END

#endif // VOLCANO_COLOR_H
