//
//
#ifndef VOLCANO_GAME_COMMON_HPP
#define VOLCANO_GAME_COMMON_HPP

#include <chrono>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <Volcano/Common.hpp>
#include <Volcano/Game/Config.hpp>

#define VOLCANO_GAME_BEGIN VOLCANO_BEGIN namespace Game {
#define VOLCANO_GAME_END } VOLCANO_END

VOLCANO_GAME_BEGIN

using Clock = std::chrono::steady_clock;

VOLCANO_GAME_END

#endif // VOLCANO_GAME_COMMON_HPP
