//
//
#ifndef VOLCANO_WORLD_TRANSFORM_H
#define VOLCANO_WORLD_TRANSFORM_H

#include <QMatrix4x4>

#include <Volcano/World/Common.h>
#include <Volcano/World/Component.h>

VOLCANO_WORLD_BEGIN

class Transformable: public Component {
    Q_OBJECT
    Q_PROPERTY(QMatrix4x4* transform READ transform)

public:
    Q_INVOKABLE Transformable(QObject* parent = nullptr)
        : Component(parent) {
    }

public:
    QMatrix4x4* transform() {
        return &transform_;
    }

    Q_INVOKABLE QMatrix4x4 absoluteTransform() const {
        if (parent_transform_ != nullptr) {
            return (*parent_transform_) * transform_;
        }
        return transform_;
    }

    void setParentTransform(const QMatrix4x4* p);

private:
    QMatrix4x4 transform_;
    const QMatrix4x4* parent_transform_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_TRANSFORM_H
