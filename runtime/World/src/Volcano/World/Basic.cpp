//
//
#include <Volcano/World/Basic.h>

VOLCANO_WORLD_BEGIN

Basic::Basic(std::string name, Eigen::Affine3f transform)
	: flags_(FlagEnabled | FlagVisible)
	, name_(std::move(name))
	, transform_(transform) {
}

VOLCANO_WORLD_END
