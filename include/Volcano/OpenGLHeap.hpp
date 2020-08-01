//
//
#ifndef VOLCANO_OPENGLHEAP_HPP
#define VOLCANO_OPENGLHEAP_HPP

#include <QList>
#include <QOpenGLBuffer>

#include <Volcano/Common.hpp>
#include <Volcano/OpenGLBuffer.hpp>

VOLCANO_BEGIN

class OpenGLHeapBuffer;

class OpenGLHeap
{
public:
    OpenGLHeap(QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage);
    virtual ~OpenGLHeap(void);

public:
    bool init(void);
    OpenGLBuffer *allocBuffer(int size);

private:
    void freeBuffer(OpenGLHeapBuffer *buf);

private:
    typedef QList<OpenGLHeapBuffer *> OpenGLBufferList;

    QOpenGLBuffer m_heap;
    OpenGLBufferList m_bufferList;
    int m_freeSize;
};

VOLCANO_END

#endif // VOLCANO_OPENGLHEAP_HPP
