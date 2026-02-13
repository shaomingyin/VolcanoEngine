//
//
#ifndef VOLCANO_WORLD_COMMON_H
#define VOLCANO_WORLD_COMMON_H

#include <chrono>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <Volcano/Common.h>
#include <Volcano/World/Config.h>

#define VOLCANO_WORLD_BEGIN VOLCANO_BEGIN namespace World {
#define VOLCANO_WORLD_END } VOLCANO_END

VOLCANO_WORLD_BEGIN

using Clock = std::chrono::steady_clock;

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_COMMON_H
