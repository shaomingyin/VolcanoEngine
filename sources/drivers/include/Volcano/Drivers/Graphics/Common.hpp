//
//
#ifndef VOLCANO_DRIVERS_GRAPHICS_COMMON_HPP
#define VOLCANO_DRIVERS_GRAPHICS_COMMON_HPP

#include <GL/gl3w.h>

#include <Volcano/Drivers/Common.hpp>

#define VOLCANO_DRIVERS_GRAPHICS_BEGIN VOLCANO_DRIVERS_BEGIN namespace Graphics {
#define VOLCANO_DRIVERS_GRAPHICS_END } VOLCANO_DRIVERS_END

VOLCANO_DRIVERS_GRAPHICS_BEGIN

struct Vertex {
    Eigen::Vector3f pos;
    Eigen::Vector3f normal;
    Eigen::Vector2f texCoord;
};

using VertexIndex = uint32_t;

VOLCANO_DRIVERS_GRAPHICS_END

#endif // VOLCANO_DRIVERS_GRAPHICS_COMMON_HPP
