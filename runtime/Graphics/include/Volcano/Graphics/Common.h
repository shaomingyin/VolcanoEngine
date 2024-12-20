//
//
#ifndef VOLCANO_GRAPHICS_COMMON_H
#define VOLCANO_GRAPHICS_COMMON_H

#include <QOpenGLFunctions_4_5_Core>

#include <Volcano/Common.h>
#include <Volcano/Graphics/Config.h>

#define VOLCANO_GRAPHICS_BEGIN VOLCANO_BEGIN namespace Graphics {
#define VOLCANO_GRAPHICS_END } VOLCANO_END

VOLCANO_GRAPHICS_BEGIN

using OpenGLFunctions = QOpenGLFunctions_4_5_Core;

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_COMMON_H
