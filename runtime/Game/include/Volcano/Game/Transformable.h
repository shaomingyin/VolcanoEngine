//
//
#ifndef VOLCANO_GAME_TRANSFORMABLE_H
#define VOLCANO_GAME_TRANSFORMABLE_H

#include <QMatrix4x4>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Component.h>

VOLCANO_GAME_BEGIN

class Transformable: public Component {
    Q_OBJECT
    Q_PROPERTY(QMatrix4x4* offset READ offset)

public:
    Q_INVOKABLE Transformable(QObject* parent = nullptr);

public:
    QMatrix4x4* offset() {
        return &transform_;
    }

    Q_INVOKABLE QMatrix4x4 absoluteTransform() const {
        return (*parent_transform_) * transform_;
    }

    void setParentTransform(const QMatrix4x4* p);

private:
    QMatrix4x4 transform_;
    const QMatrix4x4* parent_transform_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_TRANSFORMABLE_H
