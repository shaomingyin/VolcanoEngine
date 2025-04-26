//
//
#ifndef VOLCANO_GRAPHICS_COMMON_H
#define VOLCANO_GRAPHICS_COMMON_H

#include <Volcano/Common.h>
#include <Volcano/Graphics/Config.h>

#ifdef VOLCANO_GRAPHICS_SHARED
#	ifdef VOLCANO_GRAPHICS_EXPORTS
#		define VOLCANO_GRAPHICS_API VOLCANO_EXPORT
#	else
#		define VOLCANO_GRAPHICS_API VOLCANO_IMPORT
#	endif
#else
#	define VOLCANO_GRAPHICS_API
#endif

#define VOLCANO_GRAPHICS_BEGIN VOLCANO_BEGIN namespace Graphics {
#define VOLCANO_GRAPHICS_END } VOLCANO_END

#endif // VOLCANO_GRAPHICS_COMMON_H
