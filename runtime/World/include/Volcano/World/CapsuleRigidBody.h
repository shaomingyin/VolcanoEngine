//
//
#ifndef VOLCANO_WORLD_CAPSULERIGIDBODY_H
#define VOLCANO_WORLD_CAPSULERIGIDBODY_H

#include <memory>

#include <Volcano/World/Common.h>
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

class CapsuleRigidBody: public RigidBody {
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged FINAL)
    Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged FINAL)

public:
    CapsuleRigidBody(QObject* parent = nullptr);

public:
    float radius() const {
        return shape_->getRadius();
    }

    void setRadius(float v);

    float height() const {
        return (shape_->getHalfHeight() * 2.0f);
    }

    void setHeight(float v);

signals:
    void radiusChanged(float v);
    void heightChanged(float v);

private:
    std::unique_ptr<btCapsuleShape> shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CAPSULERIGIDBODY_H
