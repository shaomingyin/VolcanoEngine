/*
 *
 */
#ifndef VOLCANO_FRAMEWORK_COMMON_H
#define VOLCANO_FRAMEWORK_COMMON_H

#include <Volcano/Common.h>
#include <Volcano/Framework/Config.h>

#ifdef VOLCANO_FRAMEWORK_SHARED
#	ifdef VOLCANO_FRAMEWORK_EXPORTS
#		define VOLCANO_FRAMEWORK_API VOLCANO_EXPORT
#	else
#		define VOLCANO_FRAMEWORK_API VOLCANO_IMPORT
#endif
#else
#	define VOLCANO_FRAMEWORK_API
#endif

#define VOLCANO_FRAMEWORK_BEGIN VOLCANO_BEGIN namespace Framework {
#define VOLCANO_FRAMEWORK_END } VOLCANO_END

#endif /* VOLCANO_FRAMEWORK_COMMON_H */
