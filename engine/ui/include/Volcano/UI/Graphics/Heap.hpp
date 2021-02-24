//
//
#ifndef VOLCANO_UI_GRPHICS_HEAP_HPP
#define VOLCANO_UI_GRPHICS_HEAP_HPP

#include <QOpenGLBuffer>

#include <Volcano/UI/Graphics/Common.hpp>
#include <Volcano/UI/Graphics/Buffer.hpp>

VOLCANO_UI_GRAPHICS_BEGIN

class VOLCANO_UI_API Heap: public QObject {
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

VOLCANO_UI_GRAPHICS_END

#endif // VOLCANO_UI_GRPHICS_HEAP_HPP
