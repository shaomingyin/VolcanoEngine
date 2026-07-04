//
//
#ifndef VOLCANO_WORLD_INHERENT_H
#define VOLCANO_WORLD_INHERENT_H

#include <string>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Inherent {
public:
	Inherent() noexcept;
	Inherent(const Inherent&) = default;
	Inherent(Inherent&&) = default;

public:
	Inherent& operator=(const Inherent&) = default;
	Inherent& operator=(Inherent&&) = default;

	std::string& name() noexcept {
		return name_;
	}

	const std::string& name() const noexcept {
		return name_;
	}

	bool isEnabled() const {
		return (flags_ & FlagEnabled) != 0;
	}

	void enable() {
		flags_ |= FlagEnabled;
	}

	void disable() {
		flags_ &= ~FlagEnabled;
	}

	bool isVisible() const {
		return (flags_ & FlagVisible) != 0;
	}

	void show() {
		flags_ |= FlagVisible;
	}

	void hide() {
		flags_ &= ~FlagVisible;
	}

	friend void to_json(nlohmann::json& json, const Inherent& v);
	friend void from_json(const nlohmann::json& json, Inherent& v);

private:
	enum {
		FlagEnabled = 0x1,
		FlagVisible = 0x2
	};

	std::string name_;
	int flags_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_INHERENT_H
