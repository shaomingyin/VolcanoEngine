//
//
#include <memory>

#include <Jolt/Jolt.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/CylinderShape.h>
#include <Jolt/Physics/Collision/Shape/PlaneShape.h>

#include <Volcano/Math.h>
#include <Volcano/Physics/Common.h>

VOLCANO_PHYSICS_BEGIN

void init() {
	spdlog::info("Initializing physics system...");
	JPH::RegisterDefaultAllocator();
	JPH::Factory::sInstance = new JPH::Factory();
	JPH::RegisterTypes();
}

void shutdown() {
	spdlog::info("Shutting down physics system...");
	delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}

VOLCANO_PHYSICS_END
