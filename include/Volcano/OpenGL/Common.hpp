//
//
#ifndef VOLCANO_OPENGL_COMMON_HPP
#define VOLCANO_OPENGL_COMMON_HPP

#include <QOpenGLFunctions_3_3_Core>

#include <Volcano/Common.hpp>

#define VOLCANO_OPENGL_BEGIN VOLCANO_BEGIN namespace OpenGL {
#define VOLCANO_OPENGL_END } VOLCANO_END

VOLCANO_OPENGL_BEGIN

typedef QOpenGLFunctions_3_3_Core Functions;

struct Vertex
{
    struct { float x, y, z; } positin;
    struct { float x, y, z; } normal;
    struct { float u, v, w; } texCoord;
};

typedef quint32 VertexIndex;

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_COMMON_HPP
