//
//
#ifndef VOLCANO_DETAILS_COMMON_HPP
#define VOLCANO_DETAILS_COMMON_HPP

#include <Volcano/Details/Config.hpp>

#define VOLCANO_DETAILS_INLINE inline

#ifndef VOLCANO_DEBUG
#   define QT_NO_DEBUG_OUTPUT
#endif

#include <QtGlobal>
#include <QtConcurrent>

#include <QDebug>

#ifdef VOLCANO
#   define VOLCANO_DETAILS_API Q_DECL_EXPORT
#else
#   define VOLCANO_DETAILS_API //Q_DECL_IMPORT
#endif

#define VOLCANO_DETAILS_BEGIN namespace Volcano { namespace Details {
#define VOLCANO_DETAILS_END } }

#endif // VOLCANO_DETAILS_COMMON_HPP
