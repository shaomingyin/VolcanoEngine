//
//
#ifndef VOLCANO_GAME_TRANSFORMABLE_H
#define VOLCANO_GAME_TRANSFORMABLE_H

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Transform.h>
#include <Volcano/Game/Component.h>

VOLCANO_GAME_BEGIN

class Transformable: public Component {
    Q_OBJECT
    Q_PROPERTY(Transform* offset READ offset)

public:
    Q_INVOKABLE Transformable(QObject* parent = nullptr);

public:
    Transform* offset() {
        return &transform_;
    }

    Q_INVOKABLE Transform absoluteTransform() const {
        return (*parent_transform_) * transform_.affine();
    }

    void attachParentTransform(const Transform* p);

    friend QDataStream& operator<<(QDataStream& s, const Transformable& v);
    friend QDataStream& operator>>(QDataStream& s, Transformable& v);

private:
    Transform transform_;
    const Transform* parent_transform_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_TRANSFORMABLE_H
