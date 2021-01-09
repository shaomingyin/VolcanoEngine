//
//
#ifndef VOLCANO_COMMON_HPP
#define VOLCANO_COMMON_HPP

#include <cstdint>
#include <list>
#include <vector>
#include <string>
#include <chrono>
#include <functional>

#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <Volcano/Common.h>

#ifdef max
#    undef max
#endif

#ifdef min
#    undef min
#endif

#include <sigslot/signal.hpp>

#define VOLCANO_DISABLE_COPY(className) \
	className(const className &) = delete; \
	className &operator=(const className &) = delete

#define VOLCANO_DISABLE_MOVE(className) \
	className(className &&) = delete; \
	className &operator=(className &&) = delete

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

VOLCANO_BEGIN

using ByteArray = std::vector<uint8_t> ;
using StringList = std::list<std::string>;
using Clock = std::chrono::high_resolution_clock;
using Duration = Clock::duration;
using TimePoint = Clock::time_point;
using Nanoseconds = std::chrono::nanoseconds;
using Microseconds = std::chrono::microseconds;
using Milliseconds = std::chrono::milliseconds;
using Callback = std::function<void (void)>;

template <typename... T>
using signal = sigslot::signal_st<T...>;

VOLCANO_END

#endif // VOLCANO_COMMON_HPP
