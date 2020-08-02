//
//
#ifndef VOLCANO_OPENGLMEMORY_HPP
#define VOLCANO_OPENGLMEMORY_HPP

#include <QList>

#include <Volcano/Common.hpp>
#include <Volcano/OpenGLBuffer.hpp>
#include <Volcano/OpenGLHeap.hpp>

VOLCANO_BEGIN

class VOLCANO_API OpenGLMemory
{
public:
    OpenGLMemory(void);
    virtual ~OpenGLMemory(void);

public:
    bool init(void);
    OpenGLBuffer *allocStaticVertexBuffer(int count);
    OpenGLBuffer *allocStaticVertexIndexBuffer(int count);
    OpenGLBuffer *allocDynamicVertexBuffer(int count);
    OpenGLBuffer *allocDynamicVertexIndexBuffer(int count);

private:
    typedef QList<OpenGLHeap *> OpenGLHeapList;
    OpenGLBuffer *allocBuffer(OpenGLHeapList &heapList, QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage, int size);

private:
    OpenGLHeapList m_staticVertexHeapList;
    OpenGLHeapList m_staticVertexIndexHeapList;
    OpenGLHeapList m_dynamicVertexHeapList;
    OpenGLHeapList m_dynamicVertexIndexHeapList;
};

VOLCANO_END

#endif // VOLCANO_OPENGLMEMORY_HPP
