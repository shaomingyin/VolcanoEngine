//
//
#include <Volcano/World/Transformable.h>

VOLCANO_WORLD_BEGIN

void Transformable::setParentTransform(const QMatrix4x4* p) {
    if (parent_transform_ == p) {
        return;
    }
    if (parent_transform_ != nullptr) {
        auto offset = transform_;
        transform_ = *parent_transform_ * offset;
        parent_transform_ = nullptr;
	}
	if (p != nullptr) {
        auto offset = transform_;
        transform_ = p->inverted() * offset;
        parent_transform_ = p;
    }
}

VOLCANO_WORLD_END
