//
//
#ifndef VOLCANO_WORLD_PHYSICS_H
#define VOLCANO_WORLD_PHYSICS_H

#include <memory>

#include <entt/entt.hpp>

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include <Volcano/Math.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

inline Eigen::Vector3f fromJPH(JPH::Vec3 v) noexcept {
    return Eigen::Vector3f(v.GetX(), v.GetY(), v.GetZ());
}

inline JPH::Vec3 toJPH(Eigen::Vector3f v) noexcept {
    return JPH::Vec3(v.x(), v.y(), v.z());
}

inline Eigen::Quaternionf fromJPH(JPH::Quat v) noexcept {
    return Eigen::Quaternionf(v.GetW(), v.GetX(), v.GetY(), v.GetZ());
}

inline JPH::Quat toJPH(Eigen::Quaternionf v) noexcept {
    return JPH::Quat(v.w(), v.x(), v.y(), v.z());
}

namespace PhysicsLayers {
	static constexpr JPH::ObjectLayer NON_MOVING = 0;
	static constexpr JPH::ObjectLayer MOVING = 1;
	static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
}

namespace PhysicsBroadPhaseLayers {
	static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
	static constexpr JPH::BroadPhaseLayer MOVING(1);
	static constexpr JPH::uint NUM_LAYERS = 2;
}

class PhysicsBroadPhaseLayerInterface final: public JPH::BroadPhaseLayerInterface {
public:
	PhysicsBroadPhaseLayerInterface() {
		object_to_broad_phase_[PhysicsLayers::NON_MOVING] = PhysicsBroadPhaseLayers::NON_MOVING;
		object_to_broad_phase_[PhysicsLayers::MOVING] = PhysicsBroadPhaseLayers::MOVING;
	}

	JPH::uint GetNumBroadPhaseLayers() const override {
		return PhysicsBroadPhaseLayers::NUM_LAYERS;
	}

	JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override {
		return object_to_broad_phase_[inLayer];
	}

private:
	JPH::BroadPhaseLayer object_to_broad_phase_[PhysicsLayers::NUM_LAYERS];
};

class PhysicsObjectLayerPairFilter final: public JPH::ObjectLayerPairFilter {
public:
	bool ShouldCollide(JPH::ObjectLayer a, JPH::ObjectLayer b) const override {
		bool ret;
		switch (a) {
		case PhysicsLayers::NON_MOVING:
			ret = (b == PhysicsLayers::MOVING);
			break;
		case PhysicsLayers::MOVING:
			ret = true;
			break;
		default:
			ret = false;
			break;
		}
		return ret;
	}
};

class PhysicsObjectVsBroadPhaseLayerFilter final: public JPH::ObjectVsBroadPhaseLayerFilter {
public:
	bool ShouldCollide(JPH::ObjectLayer objectLayer, JPH::BroadPhaseLayer broadPhaseLayer) const override {
		bool ret;
		switch (objectLayer) {
		case PhysicsLayers::NON_MOVING:
			ret = (broadPhaseLayer == PhysicsBroadPhaseLayers::MOVING);
			break;
		case PhysicsLayers::MOVING:
			ret = true;
			break;
		default:
			ret = false;
			break;
		}
		return ret;
	}
};

class Physics {
public:
	Physics(entt::registry& registry);
    virtual ~Physics();

public:
	static void init();
	static void shutdown();

    const Eigen::Vector3f& gravity() const noexcept {
        return gravity_;
    }

    void setGravity(const Eigen::Vector3f& v) noexcept;
    void update(Clock::duration elapsed) noexcept;

protected:
    void onBodyAdded(entt::entity ent) noexcept;
    void onBodyRemoved(entt::entity ent) noexcept;
    void onBodyChanged(entt::entity ent) noexcept;

private:
    entt::registry& registry_;
    Eigen::Vector3f gravity_;
    PhysicsBroadPhaseLayerInterface broad_phase_layer_interface_;
    PhysicsObjectVsBroadPhaseLayerFilter object_vs_broad_phase_layer_filter_;
    PhysicsObjectLayerPairFilter object_layer_pair_filter_;
	JPH::TempAllocatorImpl temp_allocator_;
	JPH::JobSystemThreadPool job_system_;
    JPH::PhysicsSystem physics_system_;
    JPH::BodyInterface* body_interface_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_PHYSICS_H
