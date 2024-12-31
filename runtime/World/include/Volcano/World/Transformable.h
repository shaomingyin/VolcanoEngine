//
//
#ifndef VOLCANO_WORLD_TRANSFORMABLE_H
#define VOLCANO_WORLD_TRANSFORMABLE_H

#include <Volcano/World/Common.h>
#include <Volcano/World/Transform.h>
#include <Volcano/World/Component.h>

VOLCANO_WORLD_BEGIN

class Transformable: public Component {
    Q_OBJECT
    Q_PROPERTY(Transform* offset READ offset)

public:
    Q_INVOKABLE Transformable(QObject* parent = nullptr)
        : Component(parent) {
    }

public:
    Transform* offset() {
        return &transform_;
    }

    Q_INVOKABLE Affine3 absoluteTransform() const {
        if (parent_transform_ != nullptr) {
            return (*parent_transform_) * transform_.affine();
        }
        return transform_.affine();
    }

    void attachParentTransform(const Affine3* p);

private:
    Transform transform_;
    const Affine3* parent_transform_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_TRANSFORMABLE_H
