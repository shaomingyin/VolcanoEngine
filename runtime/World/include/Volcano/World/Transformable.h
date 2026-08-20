//
//
#ifndef VOLCANO_WORLD_TRANSFORMABLE_H
#define VOLCANO_WORLD_TRANSFORMABLE_H

#include <Volcano/World/Common.h>
#include <Volcano/World/Transform.h>

VOLCANO_WORLD_BEGIN

class Transformable: public QObject {
    Q_OBJECT
    Q_PROPERTY(Transform* offset)

public:
    Transformable(QObject* parent = nullptr);

public:
    Transform* offset() noexcept;

private:
    Transform offset_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_TRANSFORMABLE_H
