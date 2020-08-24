//
//
#ifndef VOLCANO_OPENGL_HPP
#define VOLCANO_OPENGL_HPP

#include <QSharedPointer>

#include <Volcano/OpenGL/Common.hpp>
#include <Volcano/OpenGL/Buffer.hpp>
#include <Volcano/OpenGL/Heap.hpp>
#include <Volcano/OpenGL/Memory.hpp>
#include <Volcano/OpenGL/Renderer.hpp>
#include <Volcano/OpenGL/View.hpp>

VOLCANO_OPENGL_BEGIN

typedef QSharedPointer<Memory> MemoryPtr;
typedef QSharedPointer<Renderer> RendererPtr;

VOLCANO_API MemoryPtr defaultMemory(void);
VOLCANO_API RendererPtr defaultRenderer(void);

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_HPP
