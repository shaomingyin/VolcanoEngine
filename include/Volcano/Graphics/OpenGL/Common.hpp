//
//
#ifndef VOLCANO_GRAPHICS_OPENGL_COMMON_HPP
#define VOLCANO_GRAPHICS_OPENGL_COMMON_HPP

#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>

#include <Volcano/Graphics/Common.hpp>

#define VOLCANO_GRAPHICS_OPENGL_BEGIN VOLCANO_GRAPHICS_BEGIN namespace OpenGL {
#define VOLCANO_GRAPHICS_OPENGL_END } VOLCANO_GRAPHICS_END

VOLCANO_GRAPHICS_OPENGL_BEGIN

typedef QOpenGLFunctions_3_3_Core Functions;

VOLCANO_INLINE Functions *functions(void)
{
    QOpenGLContext *context = QOpenGLContext::currentContext();
    if (Q_LIKELY(context != nullptr))
        return context->versionFunctions<Functions>();
    return nullptr;
}

VOLCANO_GRAPHICS_OPENGL_END

#endif // VOLCANO_GRAPHICS_OPENGL_COMMON_HPP
