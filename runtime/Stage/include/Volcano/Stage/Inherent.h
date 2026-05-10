//
//
#ifndef VOLCANO_STAGE_INHERENT_H
#define VOLCANO_STAGE_INHERENT_H

#include <string>

#include <Volcano/Stage/Common.h>

VOLCANO_STAGE_BEGIN

struct Inherent {
	enum {
		FlagEnabled = 0x1,
		FlagVisible = 0x2
	};

	std::string name;
	int flags{ FlagEnabled | FlagVisible };

	bool isEnabled() const {
		return (flags & FlagEnabled) != 0;
	}

	void enable() {
		flags |= FlagEnabled;
	}

	void disable() {
		flags &= ~FlagEnabled;
	}

	bool isVisible() const {
		return (flags & FlagVisible) != 0;
	}

	void show() {
		flags |= FlagVisible;
	}

	void hide() {
		flags &= ~FlagVisible;
	}
};

VOLCANO_STAGE_END

#endif // VOLCANO_STAGE_INHERENT_H
