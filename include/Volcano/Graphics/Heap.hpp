//
//
#ifndef VOLCANO_GRAPHICS_HEAP_HPP
#define VOLCANO_GRAPHICS_HEAP_HPP

#include <QList>
#include <QOpenGLBuffer>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Buffer.hpp>

VOLCANO_GRAPHICS_BEGIN

class HeapBuffer;

class VOLCANO_API Heap
{
public:
    Heap(QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage);
    virtual ~Heap(void);

public:
    bool init(void);
    Buffer *allocBuffer(int size);
    int freeSize(void) const;

private:
    void freeBuffer(HeapBuffer *buf);

private:
    typedef QList<HeapBuffer *> HeapBufferList;

    QOpenGLBuffer m_heap;
    HeapBufferList m_heapBufferList;
    int m_freeSize;
};

VOLCANO_INLINE int Heap::freeSize(void) const
{
    return m_freeSize;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_HEAP_HPP
