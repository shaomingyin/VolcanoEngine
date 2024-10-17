//
//
#ifndef VOLCANO_GAME_OBJECT_H
#define VOLCANO_GAME_OBJECT_H

#include <string>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Context.h>

VOLCANO_GAME_BEGIN

class Object {
public:
	Object(Context& context);
	virtual ~Object();

public:
	Context& context() {
		return context_;
	}

	const Context& context() const {
		return context_;
	}

	const std::string& name() const {
		return name_;
	}

	void setName(const std::string& v) {
		name_ = v;
	}

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

private:
	enum {
		FlagEnabled = 0x1,
		FlagVisible = 0x2
	};

	Context& context_;
	std::string name_;
	int flags_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_OBJECT_H
