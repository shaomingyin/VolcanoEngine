//
//
#ifndef VOLCANO_GRAPHICS_HEAP_HPP
#define VOLCANO_GRAPHICS_HEAP_HPP

#include <list>

#include <QIODevice>
#include <QOpenGLBuffer>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Heap {
public:
    Heap(QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage);
    virtual ~Heap(void);

public:
    bool init(int order = 26);
    QIODevice *allocBuffer(GLsizeiptr size);

private:
    void freeBuffer(QIODevice *buf);
    void bindBuffer(QIODevice *buf);
    void *mapBuffer(QIODevice *buf);
    void unmapBuffer(QIODevice *buf);

private:
    using BufferList = std::list<QIODevice *>;

private:
    QOpenGLBuffer m_glBuffer;
    QOpenGLBuffer::UsagePattern m_usage;
    BufferList m_bufferList;
    GLsizeiptr m_freeSize;
    void *m_map;
    int m_mapCount;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_HEAP_HPP
