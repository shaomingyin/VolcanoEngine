//
//
#ifndef VOLCANO_GRAPHICS_HEAP_HPP
#define VOLCANO_GRAPHICS_HEAP_HPP

#include <QList>
#include <QOpenGLBuffer>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Buffer.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_API Heap
{
public:
    Heap(QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage);
    virtual ~Heap(void);

public:
    bool init(int size);
    Buffer *allocBuffer(int size);
    const QOpenGLBuffer &glBuffer(void) const;

private:
    void freeBuffer(Buffer *buf);
    void bindBuffer(Buffer *buf);
    void *mapBuffer(Buffer *buf, QOpenGLBuffer::Access access);
    void unmapBuffer(Buffer *buf);

private:
    typedef QList<Buffer *> BufferList;

    QOpenGLBuffer m_glBuffer;
    BufferList m_bufferList;
    int m_size;
    int m_freeSize;
    void *m_map;
    int m_mapCount;
};

VOLCANO_INLINE const QOpenGLBuffer &Heap::glBuffer(void) const
{
    return m_glBuffer;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_HEAP_HPP
