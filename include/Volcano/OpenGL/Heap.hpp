//
//
#ifndef VOLCANO_OPENGL_HEAP_HPP
#define VOLCANO_OPENGL_HEAP_HPP

#include <QList>
#include <QOpenGLBuffer>

#include <Volcano/OpenGL/Common.hpp>
#include <Volcano/OpenGL/Buffer.hpp>

VOLCANO_OPENGL_BEGIN

class VOLCANO_API Heap
{
public:
    Heap(QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage);
    virtual ~Heap(void);

public:
    bool init(int size);
    Buffer *allocBuffer(int size);

private:
    void freeBuffer(Buffer *buf);
    void *mapBuffer(Buffer *buf, QOpenGLBuffer::Access access);
    void unmapBuffer(Buffer *buf);

private:
    typedef QList<Buffer *> BufferList;

    QOpenGLBuffer m_heap;
    BufferList m_bufferList;
    int m_size;
    int m_freeSize;
    void *m_map;
    int m_mapCount;
};

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_HEAP_HPP
