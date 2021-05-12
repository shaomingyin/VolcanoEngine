//
//
#ifndef VOLCANO_LAUNCHER_COMMON_HPP
#define VOLCANO_LAUNCHER_COMMON_HPP

#include <chrono>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <Volcano/Common.hpp>
#include <Volcano/Launcher/Config.hpp>

#define VOLCANO_LAUNCHER_BEGIN VOLCANO_BEGIN namespace Launcher {
#define VOLCANO_LAUNCHER_END } VOLCANO_END

VOLCANO_LAUNCHER_BEGIN

using Clock = std::chrono::high_resolution_clock;
using Duration = Clock::duration;
using TimePoint = Clock::time_point;

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_COMMON_HPP
