//
//
#ifndef VOLCANO_GRAPHICS_OPENGL_BUFFERHEAP_HPP
#define VOLCANO_GRAPHICS_OPENGL_BUFFERHEAP_HPP

#include <QObject>
#include <QOpenGLBuffer>

#include <Volcano/Graphics/OpenGL/Common.hpp>
#include <Volcano/Graphics/OpenGL/Buffer.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

class BufferImpl;

class BufferHeap {
public:
    BufferHeap(QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage);
    virtual ~BufferHeap(void);

public:
    bool init(int order = 26);
    Buffer *allocBuffer(GLsizeiptr size);

private:
    void freeBuffer(BufferImpl *buf);
    void bindBuffer(BufferImpl *buf);
    void *mapBuffer(BufferImpl *buf);
    void unmapBuffer(BufferImpl *buf);

private:
    using BufferList = std::list<BufferImpl *>;

private:
    QOpenGLBuffer m_glBuffer;
    QOpenGLBuffer::UsagePattern m_usage;
    BufferList m_bufferList;
    GLsizeiptr m_freeSize;
    void *m_map;
    int m_mapCount;
};

VOLCANO_GRAPHICS_OPENGL_END

#endif // VOLCANO_GRAPHICS_OPENGL_BUFFERHEAP_HPP
