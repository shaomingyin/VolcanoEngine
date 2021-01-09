//
//
#ifndef VOLCANO_CLIENT_GRAPHICS_COMMON_HPP
#define VOLCANO_CLIENT_GRAPHICS_COMMON_HPP

#include <GL/gl3w.h>

#include <Volcano/Common.hpp>

#define VOLCANO_CLIENT_GRAPHICS_BEGIN VOLCANO_BEGIN namespace Graphics {
#define VOLCANO_CLIENT_GRAPHICS_END } VOLCANO_END

VOLCANO_CLIENT_GRAPHICS_BEGIN

typedef struct {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tex_coord;
} Vertex;

typedef uint32_t VertexIndex;

VOLCANO_CLIENT_GRAPHICS_END

#endif // VOLCANO_CLIENT_GRAPHICS_COMMON_HPP
