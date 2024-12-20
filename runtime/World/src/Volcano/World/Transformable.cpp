//
//
#include <Volcano/World/Transformable.h>

VOLCANO_WORLD_BEGIN

void Transformable::attachWorldTransform(const QMatrix4x4* p) {
    if (world_transform_ == p) {
        return;
    }
    if (world_transform_ != nullptr) {
        auto offset = transform_;
        transform_ = *world_transform_ * offset;
        world_transform_ = nullptr;
	}
	if (p != nullptr) {
        auto offset = transform_;
        transform_ = p->inverted() * offset;
        world_transform_ = p;
        emit worldTransformChanged(*p);
    } else {
        emit worldTransformChanged(transform_);
    }
}

VOLCANO_WORLD_END
