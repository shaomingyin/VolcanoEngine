//
//
#ifndef VOLCANO_COMMON_H
#define VOLCANO_COMMON_H

#include <Eigen/Core>

#include <async++.h>

#include <SFML/System.hpp>

#include <physfs.h>

#include <Volcano/Config.h>

#ifdef VOLCANO_DEBUG
#   include <cassert>
#   define VOLCANO_ASSERT(expr) assert(expr)
#else
#   define VOLCANO_ASSERT(expr)
#endif

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

#endif // VOLCANO_COMMON_H
