//
//
#ifndef VOLCANO_GRAPHICS_COMMON_HPP
#define VOLCANO_GRAPHICS_COMMON_HPP

#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>

#include <Volcano/Common.hpp>

#define VOLCANO_GRAPHICS_BEGIN VOLCANO_BEGIN namespace Graphics {
#define VOLCANO_GRAPHICS_END } VOLCANO_END

VOLCANO_GRAPHICS_BEGIN

struct Vertex
{
    struct { float x, y, z; } positin;
    struct { float x, y, z; } normal;
    struct { float u, v, w; } texCoord;
};

typedef quint32 VertexIndex;

typedef QOpenGLFunctions_3_3_Core OpenGLFunctions;

VOLCANO_INLINE OpenGLFunctions *glFunctions(void)
{
    QOpenGLContext *context = QOpenGLContext::currentContext();
    if (Q_LIKELY(context != nullptr))
        return context->versionFunctions<OpenGLFunctions>();
    return nullptr;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_COMMON_HPP
