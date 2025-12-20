//
//
#include <Volcano/Game/Transformable.h>

VOLCANO_GAME_BEGIN

Transformable::Transformable(QObject* parent)
    : Component(parent)
    , parent_transform_(nullptr) {
}

void Transformable::setParentTransform(const QMatrix4x4* p) {
    if (parent_transform_ == p) {
        return;
    }
    if (parent_transform_ != nullptr) {
        auto offset = transform_;
        transform_ = *parent_transform_ * offset;
	}
	if (p != nullptr) {
        auto offset = transform_;
        transform_ = p->inverted() * offset;
        parent_transform_ = p;
    } else {
        parent_transform_ = nullptr;
    }
}

VOLCANO_GAME_END
