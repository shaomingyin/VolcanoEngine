//
//
#include <Volcano/Transformable.h>

VOLCANO_BEGIN

void Transformable::setParentTransform(const Affine3f* p) {
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
        transform_ = p->inverse() * offset;
        parent_transform_ = p;
    }
}

VOLCANO_END
