//
//
#ifndef VOLCANO_WORLD_BASIC_H
#define VOLCANO_WORLD_BASIC_H

#include <string>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Basic: public btMotionState {
public:
	Basic(std::string name, Eigen::Affine3f transform = Eigen::Affine3f::Identity());
	virtual ~Basic() = default;

public:
	bool isEnabled() const {
		return (flags_ & FlagEnabled);
	}

	void enable() {
		flags_ |= FlagEnabled;
	}

	void disable() {
		flags_ &= ~FlagEnabled;
	}

	bool isVisible() const {
		return (flags_ & FlagVisible);
	}

	void show() {
		flags_ |= FlagVisible;
	}

	void hide() {
		flags_ &= ~FlagVisible;
	}

	const std::string& name() const {
		return name_;
	}

	void rename(std::string v) {
		name_ = std::move(v);
	}

	Eigen::Affine3f transform() const {
		return transform_;
	}

	void setTransform(Eigen::Affine3f v) {
		transform_ = v;
	}

public:
	void getWorldTransform(btTransform& world_trans) const override;
	void setWorldTransform(const btTransform& world_trans) override;

private:
	enum {
		FlagEnabled = 0x1,
		FlagVisible = 0x2
	};

private:
	int flags_;
	std::string name_;
	Eigen::Affine3f transform_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_BASIC_H
