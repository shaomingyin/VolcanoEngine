//
//
#ifndef VOLCANO_PHYSICS_COMMON_H
#define VOLCANO_PHYSICS_COMMON_H

#include <Jolt/Jolt.h>

#include <Volcano/Common.h>
#include <Volcano/Math.h>
#include <Volcano/Physics/Config.h>

#define VOLCANO_PHYSICS_BEGIN VOLCANO_BEGIN namespace Physics {
#define VOLCANO_PHYSICS_END } VOLCANO_END

VOLCANO_PHYSICS_BEGIN

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

void init();
void shutdown();

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_COMMON_H
