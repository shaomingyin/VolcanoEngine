//
//
#ifndef VOLCANO_GAME_GRAPHICS_MEMORY_HPP
#define VOLCANO_GAME_GRAPHICS_MEMORY_HPP

#include <QIODevice>

#include <Volcano/Game/Graphics/Common.hpp>

VOLCANO_GAME_GRAPHICS_BEGIN

QIODevice *allocStaticVertices(int count);
QIODevice *allocStaticVertexIndices(int count);
QIODevice *allocDynamicVertices(int count);
QIODevice *allocDynamicVertexIndices(int count);

VOLCANO_GAME_GRAPHICS_END

#endif // VOLCANO_GAME_GRAPHICS_MEMORY_HPP
