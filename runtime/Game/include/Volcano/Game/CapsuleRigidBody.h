//
//
#ifndef VOLCANO_GAME_CAPSULERIGIDBODY_H
#define VOLCANO_GAME_CAPSULERIGIDBODY_H

#include <memory>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/RigidBody.h>

VOLCANO_GAME_BEGIN

class CapsuleRigidBody: public RigidBody {
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged FINAL)
    Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged FINAL)

public:
    CapsuleRigidBody(QObject* parent = nullptr);

public:
    float radius() const {
        return radius_;
    }


    float height() const {
        return height_;
    }

    void setRadius(float v);
    void setHeight(float v);
    void componentComplete() override;

    friend QDataStream& operator<<(QDataStream& s, const CapsuleRigidBody& v);
    friend QDataStream& operator>>(QDataStream& s, CapsuleRigidBody& v);

signals:
    void radiusChanged(float v);
    void heightChanged(float v);

private:
    float radius_;
    float height_;
    std::unique_ptr<btCapsuleShape> shape_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CAPSULERIGIDBODY_H
