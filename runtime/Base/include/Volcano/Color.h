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

	Color(int r, int g, int b, int a = 255)
		: rgba_(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f) {
	}

	Color(Eigen::Vector3f v)
		: rgba_(v.x(), v.y(), v.z(), 1.0f) {
	}

	Color(Eigen::Vector4f v)
		: rgba_(v) {
	}

public:
	Color& operator=(const Color&) = default;

	Color& operator=(Eigen::Vector3f v) {
		set(v);
	}

	Color& operator=(Eigen::Vector4f v) {
		set(v);
	}

	float red() const {
		return rgba_.x();
	}

	void setRed(float v) {
		rgba_.x() = v;
	}

	void setRed(int v) {
		setRed(v / 255.0f);
	}

	float green() const {
		return rgba_.y();
	}

	void setGreen(float v) {
		rgba_.y() = v;
	}

	void setGreen(int v) {
		setGreen(v / 255.0f);
	}

	float blue() const {
		return rgba_.z();
	}

	void setBlue(float v) {
		rgba_.z() = v;
	}

	void setBlue(int v) {
		setBlue(v / 255.0f);
	}

	float alpha() const {
		return rgba_.w();
	}

	void setAlpha(float v) {
		rgba_.w() = v;
	}

	void setAlpha(int v) {
		setAlpha(v / 255.0f);
	}

	void set(float r, float g, float b, float a = 1.0f) {
		rgba_ = Eigen::Vector4f(r, g, b, a);
	}

	void set(int r, int g, int b, int a = 255) {
		rgba_ = Eigen::Vector4f(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

	void set(Eigen::Vector3f v) {
		rgba_ = Eigen::Vector4f(v.x(), v.y(), v.z(), 1.0f);
	}

	void set(Eigen::Vector4f v) {
		rgba_ = v;
	}

	void set(Eigen::Vector3i v) {
		set(v.x() / 255.0f, v.y() / 255.0f, v.z() / 255.0f);
	}

	void set(Eigen::Vector4i v) {
		set(v.x() / 255.0f, v.y() / 255.0f, v.z() / 255.0f, v.w() / 255.0f);
	}

private:
	Eigen::Vector4f rgba_;
};

VOLCANO_END

#endif // VOLCANO_COLOR_H
