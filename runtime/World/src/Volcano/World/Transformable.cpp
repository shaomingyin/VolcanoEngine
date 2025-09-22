//
//
#include <Volcano/World/Transformable.h>

VOLCANO_WORLD_BEGIN

Transformable::Transformable(QObject* parent)
    : Component(parent)
    , parent_transform_(&Transform::identity()) {
}

void Transformable::attachParentTransform(const Transform* p) {
    if (parent_transform_ == p) {
        return;
    }
    if (parent_transform_ != nullptr) {
        auto offset = transform_.affine();
        transform_ = *parent_transform_ * offset;
	}
	if (p != nullptr) {
        auto offset = transform_.affine();
        transform_ = p->inverted() * offset;
        parent_transform_ = p;
    } else {
        parent_transform_ = &Transform::identity();
    }
}

VOLCANO_WORLD_END
