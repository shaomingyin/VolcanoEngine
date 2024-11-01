//
//
#ifndef VOLCANO_SIZE_H
#define VOLCANO_SIZE_H

#include <type_traits>

#include <Volcano/Common.h>

VOLCANO_BEGIN

template <typename T>
class Size {
	static_assert(std::is_scalar<T>::value);

public:
	Size() {
	}

	Size(T w, T h)
		: width_(w)
		, height_(h) {
	}

	Size(const Size& other)
		: width_(other.width_)
		, height_(other.height_) {
	}

public:
	T width() const {
		return width_;
	}

	T height() const {
		return height_;
	}

	void setWidth(T v) {
		width_ = v;
	}

	void setHeight(T v) {
		height_ = v;
	}

	void set(T w, T h) {
		width_ = w;
		height_ = h;
	}

	Size& operator=(const Size& other) {
		width_ = other.width_;
		height_ = other.height_;
		return (*this);
	}

private:
	T width_;
	T height_;
};

VOLCANO_END

#endif // VOLCANO_SIZE_H
