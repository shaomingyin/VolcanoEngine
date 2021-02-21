//
//
#ifndef VOLCANO_GRPHICS_HEAP_HPP
#define VOLCANO_GRPHICS_HEAP_HPP

#include <QOpenGLBuffer>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Buffer.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_GRAPHICS_API Heap: public QObject {
    Q_OBJECT

public:
    Heap(QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage, QObject *parent = nullptr);
    ~Heap(void) override;

public:
    bool init(int order = 26);
    void release(void);
    Buffer *allocBuffer(GLsizeiptr size);

private:
    void freeBuffer(Buffer *buf);
    void bindBuffer(Buffer *buf);
    void *mapBuffer(Buffer *buf);
    void unmapBuffer(Buffer *buf);

    private:
    using BufferList = QList<Buffer *>;

private:
    QOpenGLBuffer m_glBuffer;
    BufferList m_bufferList;
    GLsizeiptr m_freeSize;
    void *m_map;
    int m_mapCount;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRPHICS_HEAP_HPP
