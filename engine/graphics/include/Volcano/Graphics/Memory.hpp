//
//
#ifndef VOLCANO_GRAPHICS_MEMORY_HPP
#define VOLCANO_GRAPHICS_MEMORY_HPP

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

QIODevice *allocStaticVertexBuffer(int count);
QIODevice *allocStaticVertexIndexBuffer(int count);
QIODevice *allocDynamicVertexBuffer(int count);
QIODevice *allocDynamicVertexIndexBuffer(int count);
QIODevice *allocTexture(int width, int height, GLenum format);

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MEMORY_HPP
