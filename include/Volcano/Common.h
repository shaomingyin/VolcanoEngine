//
//
#ifndef VOLCANO_COMMON_H
#define VOLCANO_COMMON_H

#include <QtGlobal>
#include <QDebug>

#include <Volcano/Config.h>

#if defined(VOLCANO_CORE)
#   define VOLCANO_API Q_DECL_EXPORT
#else
#   define VOLCANO_API Q_DECL_IMPORT
#endif

#define VOLCANO_INLINE inline

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

#endif // VOLCANO_COMMON_H
