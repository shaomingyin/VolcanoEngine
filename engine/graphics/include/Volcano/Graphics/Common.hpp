//
//
#ifndef VOLCANO_GRAPHICS_COMMON_HPP
#define VOLCANO_GRAPHICS_COMMON_HPP

#include <QVector2D>
#include <QVector3D>
#include <QOpenGLContext>
#include <QOpenGLVersionFunctionsFactory>
#include <QOpenGLFunctions_4_0_Core>

#include <Volcano/Common.hpp>

#ifdef VOLCANO_GRAPHICS
#   define VOLCANO_GRAPHICS_API Q_DECL_EXPORT
#else
#   define VOLCANO_GRAPHICS_API
#endif

#define VOLCANO_GRAPHICS_BEGIN VOLCANO_BEGIN namespace Graphics {
#define VOLCANO_GRAPHICS_END } VOLCANO_END

VOLCANO_GRAPHICS_BEGIN

struct Vertex {
    QVector3D pos;
    QVector3D normal;
    QVector2D texCoord;
};

using VertexIndex = quint32;

using OpenGLFunctions = QOpenGLFunctions_4_0_Core;

VOLCANO_INLINE OpenGLFunctions *glFunctions(void)
{
    static thread_local OpenGLFunctions *gl = nullptr;

    if (Q_LIKELY(gl != nullptr))
        return gl;

    auto glContext = QOpenGLContext::currentContext();
    if (glContext != nullptr)
        gl = QOpenGLVersionFunctionsFactory::get<OpenGLFunctions>(glContext);

    return gl;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_COMMON_HPP
