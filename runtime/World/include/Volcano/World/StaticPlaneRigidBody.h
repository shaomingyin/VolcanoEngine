//
//
#ifndef VOLCANO_WORLD_STATICPLANERIGIDBODY_H
#define VOLCANO_WORLD_STATICPLANERIGIDBODY_H

#include <memory>

#include <Volcano/World/Common.h>
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

class StaticPlaneRigidBody: public RigidBody {
    Q_OBJECT
    Q_PROPERTY(Vector3 normal READ normal WRITE setNormal NOTIFY normalChanged FINAL)
    Q_PROPERTY(float constant READ constant WRITE setConstant NOTIFY constantChanged FINAL)

public:
    StaticPlaneRigidBody(QObject* parent = nullptr);

public:
    Vector3 normal() const {
        return normal_;
    }


    float constant() const {
        return constant_;
    }

    void setNormal(const Vector3& v);
    void setConstant(float v);
    void componentComplete() override;

signals:
    void normalChanged(const Vector3& v);
    void constantChanged(float v);

private:
    Vector3 normal_;
    float constant_;
    std::unique_ptr<btStaticPlaneShape> shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_STATICPLANERIGIDBODY_H
