//
//
#include <Volcano/World/Inherency.h>

VOLCANO_WORLD_BEGIN

Inherency::Inherency(std::string name, Eigen::Affine3f transform)
	: flags_(FlagEnabled | FlagVisible)
	, name_(std::move(name))
	, transform_(transform) {
}

VOLCANO_WORLD_END
