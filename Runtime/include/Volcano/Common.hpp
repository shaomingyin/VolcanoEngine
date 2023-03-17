//
//
#ifndef VOLCANO_COMMON_HPP
#define VOLCANO_COMMON_HPP

#include <QtGlobal>
#include <QtConcurrent>

#include <Volcano/Config.hpp>

#ifdef VOLCANO_DEBUG
#   define VOLCANO_ASSERT(expr) Q_ASSERT(expr)
#else
#   define VOLCANO_ASSERT(expr)
#endif

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

#endif // VOLCANO_COMMON_HPP
