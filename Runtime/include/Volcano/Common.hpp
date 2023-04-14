//
//
#ifndef VOLCANO_COMMON_HPP
#define VOLCANO_COMMON_HPP

#include <chrono>

#include <QtGlobal>
#include <QtConcurrent>

#include <Volcano/Config.hpp>

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

VOLCANO_BEGIN

using Clock = std::chrono::high_resolution_clock;
using TimePoint = Clock::time_point;
using Duration = Clock::duration;

VOLCANO_END

#endif // VOLCANO_COMMON_HPP
