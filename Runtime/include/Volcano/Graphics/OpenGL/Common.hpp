//
//
#ifndef VOLCANO_GRAPHICS_OPENGL_COMMON_HPP
#define VOLCANO_GRAPHICS_OPENGL_COMMON_HPP

#include <QOpenGLFunctions_3_3_Core>

#include <Volcano/Graphics/Common.hpp>

#define VOLCANO_GRAPHICS_OPENGL_BEGIN VOLCANO_GRAPHICS_BEGIN namespace OpenGL {
#define VOLCANO_GRAPHICS_OPENGL_END } VOLCANO_GRAPHICS_END

VOLCANO_GRAPHICS_OPENGL_BEGIN

using Functions = QOpenGLFunctions_3_3_Core;

void initDefaultSettings(void);

VOLCANO_GRAPHICS_OPENGL_END

#endif // VOLCANO_GRAPHICS_OPENGL_COMMON_HPP
