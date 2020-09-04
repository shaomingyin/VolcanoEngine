//
//
#ifndef VOLCANO_DRIVERS_GRAPHICS_COMMON_HPP
#define VOLCANO_DRIVERS_GRAPHICS_COMMON_HPP

#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>

#include <Volcano/Drivers/Common.hpp>

#define VOLCANO_DRIVERS_GRAPHICS_BEGIN VOLCANO_DRIVERS_BEGIN namespace Graphics {
#define VOLCANO_DRIVERS_GRAPHICS_END } VOLCANO_DRIVERS_END

VOLCANO_DRIVERS_GRAPHICS_BEGIN

typedef QOpenGLFunctions_3_3_Core Functions;

struct Vertex
{
    struct { float x, y, z; } position;
    struct { float x, y, z; } normal;
    struct { float u, v, w; } texCoord;
};

typedef quint32 VertexIndex;

VOLCANO_DRIVERS_GRAPHICS_END

#endif // VOLCANO_DRIVERS_GRAPHICS_COMMON_HPP
