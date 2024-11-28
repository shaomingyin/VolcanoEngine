//
//
#ifndef VOLCANO_WORLD_INHERENCY_H
#define VOLCANO_WORLD_INHERENCY_H

#include <string>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Inherency {
public:
	Inherency(std::string name, const Eigen::Affine3f& trans)
		: flags_(FlagEnabled)
		, name_(std::move(name))
		, transform_(Eigen::Affine3f::Identity()) {
	}

	Inherency(std::string name)
		: Inherency(std::move(name), Eigen::Affine3f::Identity()) {
	}

	Inherency(const Inherency&) = default;
	Inherency(Inherency&&) = default;
	Inherency& operator=(const Inherency&) = default;
	Inherency& operator=(Inherency&&) = default;

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

	std::string& name() {
		return name_;
	}

	const std::string& name() const {
		return name_;
	}

	Eigen::Affine3f& transform() {
		return transform_;
	}

	const Eigen::Affine3f& transform() const {
		return transform_;
	}

private:
	enum {
		FlagEnabled = 0x1
	};

	int flags_;
	std::string name_;
	Eigen::Affine3f transform_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_INHERENCY_H
