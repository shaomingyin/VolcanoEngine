//
//
#ifndef VOLCANO_GRAPHICS_COMMON_HPP
#define VOLCANO_GRAPHICS_COMMON_HPP

#include <GL/gl3w.h>
#include <nanovg.h>

#include <Volcano/Common.hpp>

#define VOLCANO_GRAPHICS_BEGIN VOLCANO_BEGIN namespace Graphics {
#define VOLCANO_GRAPHICS_END } VOLCANO_END

VOLCANO_GRAPHICS_BEGIN

struct Vertex {
    Eigen::Vector3f pos;
    Eigen::Vector3f normal;
    Eigen::Vector2f texCoord;
};

using VertexIndex = uint32_t;

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_COMMON_HPP
