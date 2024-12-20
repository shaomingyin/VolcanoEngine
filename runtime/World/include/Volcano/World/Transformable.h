//
//
#ifndef VOLCANO_WORLD_TRANSFORMABLE_H
#define VOLCANO_WORLD_TRANSFORMABLE_H

#include <QMatrix4x4>

#include <Volcano/World/Common.h>
#include <Volcano/World/Component.h>

VOLCANO_WORLD_BEGIN

class Transformable: public Component {
    Q_OBJECT

public:
    Q_INVOKABLE Transformable(QObject* parent = nullptr)
        : Component(parent) {
    }

public:
    const QMatrix4x4& transform() const {
        return transform_;
    }

    void setTransform(QMatrix4x4 v) {
        if (!qFuzzyCompare(transform_, v)) {
            transform_ = v;
            emit transformChanged(v);
            emit worldTransformChanged(worldTransform());
        }
    }

    QMatrix4x4 worldTransform() const {
        if (world_transform_ != nullptr) {
            return (*world_transform_) * transform_;
		}
        return transform_;
	}

    void attachWorldTransform(const QMatrix4x4* p);

signals:
    void transformChanged(const QMatrix4x4& v);
    void worldTransformChanged(const QMatrix4x4& v);

private:
    QMatrix4x4 transform_;
    const QMatrix4x4* world_transform_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_TRANSFORMABLE_H
