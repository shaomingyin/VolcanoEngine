//
//
#ifndef VOLCANO_COMMON_HPP
#define VOLCANO_COMMON_HPP

#include <cstdint>
#include <cassert>

#define GLM_FORCE_INLINE
#define GLM_FORCE_CXX17
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <Volcano/Config.h>

#ifdef VOLCANO_DEBUG
#	define VOLCANO_ASSERT(expr) assert(expr)
#else
#	define VOLCANO_ASSERT(expr)
#endif

#define VOLCANO_INLINE inline

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

VOLCANO_BEGIN

VOLCANO_END

#endif // VOLCANO_COMMON_HPP
