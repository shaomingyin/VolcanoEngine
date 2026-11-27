//
//
#ifndef VOLCANO_WORLD_COLLISIONSHAPE_H
#define VOLCANO_WORLD_COLLISIONSHAPE_H

#include <QObject>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class CollisionShape: public QObject {
    Q_OBJECT

public:
    explicit CollisionShape(QObject* parent = nullptr);
    ~CollisionShape() override;

public:
    virtual bool isValid() const noexcept = 0;

signals:
    void shapeChanged();

};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_COLLISIONSHAPE_H
