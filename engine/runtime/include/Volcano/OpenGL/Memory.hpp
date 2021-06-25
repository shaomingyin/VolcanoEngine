//
//
#ifndef VOLCANO_OPENGL_MEMORY_HPP
#define VOLCANO_OPENGL_MEMORY_HPP

#include <QIODevice>

#include <Volcano/OpenGL/Common.hpp>

VOLCANO_OPENGL_BEGIN

QIODevice *allocStaticVertices(int count);
QIODevice *allocStaticVertexIndices(int count);
QIODevice *allocDynamicVertices(int count);
QIODevice *allocDynamicVertexIndices(int count);

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_MEMORY_HPP
