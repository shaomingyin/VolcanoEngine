//
//
#ifndef VOLCANO_GRAPHICS_COMMON_HPP
#define VOLCANO_GRAPHICS_COMMON_HPP

#include <Volcano/Common.hpp>

#define VOLCANO_GRAPHICS_BEGIN VOLCANO_BEGIN namespace Graphics {
#define VOLCANO_GRAPHICS_END } VOLCANO_END

VOLCANO_GRAPHICS_BEGIN

struct Vertex {
    float position[3];
    float normal[3];
    float texCoord[2];
};

using VertexIndex = quint32;

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_COMMON_HPP
