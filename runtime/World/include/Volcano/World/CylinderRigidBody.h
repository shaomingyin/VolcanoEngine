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
    Q_PROPERTY(Vector3 size READ size WRITE resize NOTIFY sizeChanged FINAL)

public:
    CylinderRigidBody(QObject* parent = nullptr);

public:
    const Vector3& size() const {
        return size_;
    }

    void resize(const Vector3& v);
    void componentComplete() override;

    friend QDataStream& operator<<(QDataStream& s, const CylinderRigidBody& v);
    friend QDataStream& operator>>(QDataStream& s, CylinderRigidBody& v);

signals:
    void sizeChanged(const Vector3& v);

private:
    Vector3 size_;
    std::unique_ptr<btCylinderShape> shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CYLINDERRIGIDBODY_H
