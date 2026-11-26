//
//
#ifndef VOLCANO_PHYSICS_SYSTEM_H
#define VOLCANO_PHYSICS_SYSTEM_H

#include <chrono>
#include <cstdint>
#include <memory>

#include <QObject>
#include <QQuaternion>
#include <QVector3D>

#include <Volcano/Physics/Common.h>

namespace JPH {
class BodyID;
}

VOLCANO_PHYSICS_BEGIN

class SystemPrivate;
class RigidBody;

class System: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)

public:
    class BodyId {
    public:
        BodyId() = default;
        bool operator==(const BodyId&) const noexcept = default;

    private:
        friend class System;

        BodyId(const System* system, std::uint64_t generation, std::uint32_t value)
            : system_(system)
            , generation_(generation)
            , value_(value) {
        }

        const System* system_ = nullptr;
        std::uint64_t generation_ = 0;
        std::uint32_t value_ = 0;
    };

    struct BodyPose {
        QVector3D position;
        QQuaternion rotation;
    };

public:
    explicit System(QObject* parent = nullptr);
    ~System() override;

public:
    Q_INVOKABLE void enable();
    Q_INVOKABLE void disable();
    bool isEnabled() const noexcept;
    void setEnabled(bool enabled);
    const QVector3D& gravity() const noexcept;
    void setGravity(const QVector3D& gravity);
    BodyId createBody(const RigidBody& body, const QVector3D& position = {}, const QQuaternion& rotation = {});
    void addBody(BodyId body, bool activate = true);
    void removeBody(BodyId body);
    void destroyBody(BodyId body);
    bool isBodyAdded(BodyId body) const;
    BodyPose bodyPose(BodyId body) const;
    void update(std::chrono::steady_clock::duration deltaTime, int collisionSteps = 1);

signals:
    void enabledChanged(bool enabled);
    void gravityChanged(const QVector3D& gravity);

private:
    SystemPrivate& backend() const;
    JPH::BodyID checkedBodyId(BodyId body) const;

private:
    QVector3D gravity_ { 0.0f, -9.81f, 0.0f };
    std::unique_ptr<SystemPrivate> d_;
    std::uint64_t generation_ = 0;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_SYSTEM_H
