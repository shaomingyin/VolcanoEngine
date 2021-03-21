//
//
#ifndef VOLCANO_UI_GRAPHICS_COMMON_HPP
#define VOLCANO_UI_GRAPHICS_COMMON_HPP

#include <GL/gl3w.h>

#include <Volcano/UI/Common.hpp>

#define VOLCANO_UI_GRAPHICS_BEGIN VOLCANO_UI_BEGIN namespace Graphics {
#define VOLCANO_UI_GRAPHICS_END } VOLCANO_UI_END

VOLCANO_UI_GRAPHICS_BEGIN

struct Vertex {
    Eigen::Vector3f pos;
    Eigen::Vector3f normal;
    Eigen::Vector2f texCoord;
};

using VertexIndex = uint32_t;

VOLCANO_UI_GRAPHICS_END

#endif // VOLCANO_UI_GRAPHICS_COMMON_HPP
