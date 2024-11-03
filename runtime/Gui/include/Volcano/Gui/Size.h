//
//
#ifndef VOLCANO_GUI_SIZE_H
#define VOLCANO_GUI_SIZE_H

#include <type_traits>

#include <Volcano/Gui/Common.h>

VOLCANO_GUI_BEGIN

class Size final {
public:
	Size() = default;
	Size(const Size& other) = default;

	Size(Eigen::Vector2f v)
		: v_(v) {
	}

	Size(float w, float h)
		: v_(w, h) {
	}

public:
	Size& operator=(const Size& other) = default;

	Size& operator=(Eigen::Vector2f v) {
		v_ = v;
	}

	float width() const {
		return v_.x();
	}

	void setWidth(float v) {
		v_.y() = v;
	}

	float height() const {
		return v_.y();
	}

	void setHeight(float v) {
		v_.y() = v;
	}

	void set(float w, float h) {
		v_ = Eigen::Vector2f(w, h);
	}

	void set(Eigen::Vector2f v) {
		v_ = v;
	}

	Eigen::Vector2f toVector() const {
		return v_;
	}

	bool isEmpty() const {
		return v_.isApprox(Eigen::Vector2f(0.0f, 0.0f), 0.001f);
	}

private:
	Eigen::Vector2f v_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_SIZE_H
