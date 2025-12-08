//
//
#ifndef VOLCANO_GAME_CYLINDERRIGIDBODY_H
#define VOLCANO_GAME_CYLINDERRIGIDBODY_H

#include <memory>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/RigidBody.h>

VOLCANO_GAME_BEGIN

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

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CYLINDERRIGIDBODY_H
