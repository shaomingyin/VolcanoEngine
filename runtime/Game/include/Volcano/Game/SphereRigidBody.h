//
//
#ifndef VOLCANO_GAME_SPHERERIGIDBODY_H
#define VOLCANO_GAME_SPHERERIGIDBODY_H

#include <memory>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/RigidBody.h>

VOLCANO_GAME_BEGIN

class SphereRigidBody: public RigidBody {
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged FINAL)

public:
    SphereRigidBody(QObject* parent = nullptr);

public:
    float radius() const {
        return radius_;
    }

    void setRadius(float v);
    void componentComplete() override;

    friend QDataStream& operator<<(QDataStream& s, const SphereRigidBody& v);
    friend QDataStream& operator>>(QDataStream& s, SphereRigidBody& v);

signals:
    void radiusChanged(float v);

private:
    float radius_;
    std::unique_ptr<btSphereShape> shape_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SPHERERIGIDBODY_H
