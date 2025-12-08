//
//
#include <Volcano/Game/Transformable.h>

VOLCANO_GAME_BEGIN

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

QDataStream& operator<<(QDataStream& s, const Transformable& v) {
    s << static_cast<const Component&>(v);
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, Transformable& v) {
    s >> static_cast<Component&>(v);
    // TODO
    return s;
}

VOLCANO_GAME_END
