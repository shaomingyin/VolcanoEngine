//
//
#include <Volcano/World/Basic.h>

VOLCANO_WORLD_BEGIN

Basic::Basic(std::string name, Eigen::Affine3f transform)
	: flags_(FlagEnabled | FlagVisible)
	, name_(std::move(name))
	, transform_(transform) {
}

void Basic::getWorldTransform(btTransform& world_trans) const {
	world_trans.setFromOpenGLMatrix(transform_.data());
}

void Basic::setWorldTransform(const btTransform& world_trans) {
	world_trans.getOpenGLMatrix(transform_.data());
}

VOLCANO_WORLD_END
