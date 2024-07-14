//
//
#ifndef VOLCANO_COMMON_HPP
#define VOLCANO_COMMON_HPP

#include <chrono>

#include <QtGlobal>
#include <QtConcurrent>

#include <Eigen/Eigen>

#include <Volcano/Config.hpp>

#define VOLCANO_STRIZE(x) VOLCANO_STRIZE_(x)
#define VOLCANO_STRIZE_(x) #x

#define VOLCANO_VERSION_STR \
    VOLCANO_STRIZE(VOLCANO_VERSION_MAJOR) "." \
    VOLCANO_STRIZE(VOLCANO_VERSION_MINOR) "." \
    VOLCANO_STRIZE(VOLCANO_VERSION_PATCH)

#ifdef VOLCANO_BASE
#   define VOLCANO_API Q_DECL_EXPORT
#else
#   define VOLCANO_API Q_DECL_IMPORT
#endif

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

VOLCANO_BEGIN

using Clock = std::chrono::high_resolution_clock;
using TimePoint = Clock::time_point;
using Duration = Clock::duration;

VOLCANO_END

#endif // VOLCANO_COMMON_HPP
