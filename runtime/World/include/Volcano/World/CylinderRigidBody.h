//
//
#ifndef VOLCANO_WORLD_CYLINDERRIGIDBODY_H
#define VOLCANO_WORLD_CYLINDERRIGIDBODY_H

#include <memory>

#include <Volcano/World/Common.h>
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

class CylinderRigidBody: public RigidBody {
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged FINAL)
    Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged FINAL)

public:
    CylinderRigidBody(QObject* parent = nullptr);

public:
    float radius() const {
        return shape_->getRadius();
    }

    void setRadius(float v) {
        float h = shape_->getHalfExtentsWithoutMargin().y();
        shape_.reset(new btCylinderShape({ v, h, v }));
        setCollisionShape(shape_.get());
        emit radiusChanged(v);
    }

    float height() const {
        return 1.0f; // TODO
    }

    void setHeight(float v) {
        float r = shape_->getRadius();
        shape_.reset(new btCylinderShape({ r, v, r }));
        setCollisionShape(shape_.get());
        emit radiusChanged(v);
    }

signals:
    void radiusChanged(float v);
    void heightChanged(float v);

private:
    std::unique_ptr<btCylinderShape> shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CYLINDERRIGIDBODY_H
