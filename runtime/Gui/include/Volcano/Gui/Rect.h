//
//
#ifndef VOLCANO_GUI_RECT_H
#define VOLCANO_GUI_RECT_H

#include <type_traits>

#include <Volcano/Gui/Common.h>
#include <Volcano/Gui/Point.h>
#include <Volcano/Gui/Size.h>

VOLCANO_GUI_BEGIN

class Rect final {
public:
	Rect() = default;

	Rect(float x, float y, float w, float h)
		: pos_(x, y)
		, size_(w, h) {
	}

	Rect(Eigen::Vector2f pos, Eigen::Vector2f size)
		: pos_(pos)
		, size_(size) {
	}

	Rect(const Rect& other) = default;

public:
	Rect& operator=(const Rect&) = default;

	float x() const {
		return pos_.x();
	}

	void setX(float v) {
		pos_.setX(v);
	}

	float y() const {
		return pos_.y();
	}

	void setY(float v) {
		pos_.setY(v);
	}

	Point pos() const {
		return pos_;
	}

	void setPos(float x, float y) {
		pos_ = Eigen::Vector2f(x, y);
	}

	void setPos(Point v) {
		pos_ = v;
	}

	float width() const {
		return size_.width();
	}

	void setWidth(float v) {
		size_.setWidth(v);
	}

	float height() const {
		return size_.height();
	}

	void setHeight(float v) {
		size_.setHeight(v);
	}

	const Size& size() const {
		return size_;
	}

	void setSize(float w, float h) {
		size_.set(w, h);
	}

	void setSize(Size v) {
		size_ = v;
	}

	void set(float x, float y, float w, float h) {
		pos_.set(x, y);
		size_.set(w, h);
	}

	void set(Point pos, Size size) {
		pos_ = pos;
		size_ = size;
	}

	void set(Eigen::Vector4f v) {
		pos_.set(v.x(), v.y());
		size_.set(v.z(), v.w());
	}

	bool isEmpty() const {
		return size_.isEmpty();
	}

private:
	Point pos_;
	Size size_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_RECT_H
