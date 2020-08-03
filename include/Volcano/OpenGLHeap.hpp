//
//
#ifndef VOLCANO_OPENGLHEAP_HPP
#define VOLCANO_OPENGLHEAP_HPP

#include <QList>
#include <QIODevice>
#include <QOpenGLBuffer>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class OpenGLBuffer;

class VOLCANO_API OpenGLHeap
{
public:
    OpenGLHeap(QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage);
    virtual ~OpenGLHeap(void);

public:
    bool init(void);
    QIODevice *allocBuffer(int size);

private:
    void freeBuffer(OpenGLBuffer *buf);

private:
    typedef QList<OpenGLBuffer *> OpenGLBufferList;

    QOpenGLBuffer m_heap;
    OpenGLBufferList m_bufferList;
    int m_freeSize;
};

VOLCANO_END

#endif // VOLCANO_OPENGLHEAP_HPP
