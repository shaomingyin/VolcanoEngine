//
//
#ifndef VOLCANO_RENDERER_COMMON_H
#define VOLCANO_RENDERER_COMMON_H

#include <Volcano/Common.h>
#include <Volcano/Renderer/Config.h>

#define VOLCANO_RENDERER_BEGIN VOLCANO_BEGIN namespace Renderer {
#define VOLCANO_RENDERER_END } VOLCANO_END

VOLCANO_RENDERER_BEGIN

struct Vertex {
    struct { float x, y, z; } position;
    struct { float x, y, z; } normal;
    struct { float u, v; } texCoord;
};

typedef quint16 VertexIndex;

VOLCANO_RENDERER_END

#endif // VOLCANO_RENDERER_COMMON_H
