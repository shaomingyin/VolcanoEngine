//
//
#ifndef VOLCANO_GUI_POINT_H
#define VOLCANO_GUI_POINT_H

#include <type_traits>

#include <Volcano/Gui/Common.h>

VOLCANO_GUI_BEGIN

class Point final {
public:
	Point() = default;
	Point(const Point&) = default;
	Point& operator=(const Point&) = default;

	Point(float x, float y)
		: v_(x, y) {
	}

	Point(Eigen::Vector2f xy)
		: v_(xy) {
	}

public:
	float x() const {
		return v_.x();
	}

	void setX(float v) {
		v_.x() = v;
	}

	float y() const {
		return v_.y();
	}

	void setY(float v) {
		v_.y() = v;
	}

	void set(float x, float y) {
		v_.x() = x;
		v_.y() = y;
	}

	void set(Eigen::Vector2f v) {
		v_ = v;
	}

	Eigen::Vector2f toVector() const {
		return v_;
	}

private:
	Eigen::Vector2f v_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_POINT_H
