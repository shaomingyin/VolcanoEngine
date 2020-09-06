//
//
#ifndef VOLCANO_COMMON_HPP
#define VOLCANO_COMMON_HPP

#include <Volcano/Config.hpp>

#define VOLCANO_INLINE inline

#ifndef VOLCANO_DEBUG
#   define QT_NO_DEBUG_OUTPUT
#endif

#include <QtGlobal>
#include <QtConcurrent>

#include <QDebug>

#ifdef VOLCANO
#   define VOLCANO_API Q_DECL_EXPORT
#else
#   define VOLCANO_API //Q_DECL_IMPORT
#endif

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

VOLCANO_BEGIN

VOLCANO_END

#endif // VOLCANO_COMMON_HPP
