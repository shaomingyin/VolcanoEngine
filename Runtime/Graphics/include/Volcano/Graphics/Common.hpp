//
//
#ifndef VOLCANO_GRAPHICS_COMMON_HPP
#define VOLCANO_GRAPHICS_COMMON_HPP

#include <Volcano/Common.hpp>

#ifdef VOLCANO_GRAPHICS
#   define VOLCANO_GRAPHICS_API Q_DECL_EXPORT
#else
#   define VOLCANO_GRAPHICS_API Q_DECL_IMPORT
#endif

#define VOLCANO_GRAPHICS_BEGIN VOLCANO_BEGIN namespace Graphics {
#define VOLCANO_GRAPHICS_END } VOLCANO_END

#endif // VOLCANO_GRAPHICS_COMMON_HPP
