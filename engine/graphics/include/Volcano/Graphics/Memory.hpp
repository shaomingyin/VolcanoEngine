//
//
#ifndef VOLCANO_GRAPHICS_MEMORY_HPP
#define VOLCANO_GRAPHICS_MEMORY_HPP

#include <QIODevice>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

QIODevice *allocStaticVertices(int count);
QIODevice *allocStaticVertexIndices(int count);
QIODevice *allocDynamicVertices(int count);
QIODevice *allocDynamicVertexIndices(int count);

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MEMORY_HPP
