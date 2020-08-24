//
//
#ifndef VOLCANO_OPENGL_HEAP_HPP
#define VOLCANO_OPENGL_HEAP_HPP

#include <QList>
#include <QOpenGLBuffer>

#include <Volcano/OpenGL/Common.hpp>
#include <Volcano/OpenGL/Buffer.hpp>

VOLCANO_OPENGL_BEGIN

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
    void *mapBuffer(HeapBuffer *buf);
    void unmapBuffer(HeapBuffer *buf);

private:
    typedef QList<HeapBuffer *> HeapBufferList;

    QOpenGLBuffer m_heap;
    HeapBufferList m_heapBufferList;
    int m_freeSize;
    void *m_map;
    int m_mapCount;
};

VOLCANO_INLINE int Heap::freeSize(void) const
{
    return m_freeSize;
}

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_HEAP_HPP
