//
//
#ifndef VOLCANO_GAME_BASIC_H
#define VOLCANO_GAME_BASIC_H

#include <string>

#include <Volcano/Game/Common.h>

VOLCANO_GAME_BEGIN

class Basic {
public:
	Basic(std::string name, int flags = 0);
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

	Eigen::Transform3f& transform() {
		return transform_;
	}

	const Eigen::Transform3f& transform() const {
		return transform_;
	}

private:
	enum {
		FlagEnabled = 0x1,
		FlagVisible = 0x2
	};

private:
	int flags_;
	std::string name_;
	Eigen::Transform3f transform_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_BASIC_H
