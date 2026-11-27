//
//
#ifndef VOLCANO_PHYSICS_SYSTEM_H
#define VOLCANO_PHYSICS_SYSTEM_H

#include <cstdint>
#include <optional>

#include <QObject>
#include <QQuaternion>
#include <QVector3D>

#include <Jolt/Jolt.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <Volcano/Physics/Common.h>
#include <Volcano/World/Common.h>

VOLCANO_BEGIN
namespace World {
class Entity;
class RigidBody;
class Scene;
}
VOLCANO_END

VOLCANO_PHYSICS_BEGIN

class System: public QObject {
    Q_OBJECT

public:
    explicit System(World::Scene& scene, QObject* parent = nullptr);
    ~System() override;

public:
    void enable();
    void disable();
    bool isEnabled() const noexcept;
    void setEnabled(bool enabled);
    void update(World::Clock::duration deltaTime, int collisionSteps = 1);

signals:
    void enabledChanged(bool enabled);

private:
    class BroadPhaseLayerInterface final: public JPH::BroadPhaseLayerInterface {
    public:
        JPH::uint GetNumBroadPhaseLayers() const override;
        JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer layer) const override;
#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
        const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer layer) const override;
#endif
    };

    class ObjectLayerPairFilter final: public JPH::ObjectLayerPairFilter {
    public:
        bool ShouldCollide(JPH::ObjectLayer first, JPH::ObjectLayer second) const override;
    };

    class ObjectVsBroadPhaseLayerFilter final: public JPH::ObjectVsBroadPhaseLayerFilter {
    public:
        bool ShouldCollide(JPH::ObjectLayer objectLayer, JPH::BroadPhaseLayer broadPhaseLayer) const override;
    };

    struct Backend {
        Backend();

        JPH::TempAllocatorImpl tempAllocator;
        JPH::JobSystemThreadPool jobSystem;
        BroadPhaseLayerInterface broadPhaseLayerInterface;
        ObjectLayerPairFilter objectLayerPairFilter;
        ObjectVsBroadPhaseLayerFilter objectVsBroadPhaseLayerFilter;
        JPH::PhysicsSystem physicsSystem;
    };

    void createSceneBody(World::RigidBody& body, const QVector3D& position, const QQuaternion& rotation);
    void destroyNativeBody(JPH::BodyID id);
    void attachSceneBody(World::Entity* entity, QObject* component);
    void detachSceneBody(World::Entity* entity, QObject* component);
    void visitSceneComponents(void (System::*handler)(World::Entity*, QObject*));
    void syncSceneTransforms();

private:
    World::Scene& scene_;
    std::optional<Backend> backend_;
    std::uint64_t generation_ = 0;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_SYSTEM_H
