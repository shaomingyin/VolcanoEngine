//
//
#ifndef VOLCANO_OPENGLMEMORY_HPP
#define VOLCANO_OPENGLMEMORY_HPP

#include <QIODevice>

#include <Volcano/OpenGLCommon.hpp>

VOLCANO_BEGIN

QIODevice *glAllocStaticVertices(int count);
QIODevice *glAllocStaticVertexIndices(int count);
QIODevice *glAllocDynamicVertices(int count);
QIODevice *glAllocDynamicVertexIndices(int count);

VOLCANO_END

#endif // VOLCANO_OPENGLMEMORY_HPP
