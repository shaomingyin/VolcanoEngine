//
//
#ifndef VOLCANO_GRAPHICS_OPENGL_BUFFER_HPP
#define VOLCANO_GRAPHICS_OPENGL_BUFFER_HPP

#include <functional>

#include <QIODevice>
#include <QOpenGLBuffer>

#include <Volcano/Graphics/Buffer.hpp>
#include <Volcano/Graphics/OpenGL/Common.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

class Heap;

class VOLCANO_API Buffer: public Graphics::Buffer
{
    friend class Heap;

    Q_OBJECT

public:
    ~Buffer(void) override;

public:
    bool atEnd(void) const override;
    void close(void) override;
    bool isSequential(void) const override;
    bool open(QIODevice::OpenMode mode) override;
    qint64 pos(void) const override;
    bool reset(void) override;
    bool seek(qint64 pos) override;
    qint64 size(void) const override;
    qint64 readData(char *data, qint64 maxSize)  override;
    qint64 writeData(const char *data, qint64 maxSize)  override;
    int offset(void) const;
    void *data(int pos = 0);
    Heap &heap(void);
    const Heap &heap(void) const;

private:
    Buffer(Heap &heap, int offset, int size, QObject *parent = nullptr);

    std::function<void (void)> freeFunction;
    std::function<void (void)> bindFunction;
    std::function<void *(QOpenGLBuffer::Access)> mapFunction;
    std::function<void (void)> unmapFunction;

private:
    Heap &m_heap;
    int m_offset;
    int m_size;
    int m_pos;
    void *m_map;
};

VOLCANO_INLINE int Buffer::offset(void) const
{
    return m_offset;
}

VOLCANO_INLINE void *Buffer::data(int pos)
{
    Q_ASSERT(m_map != nullptr);
    Q_ASSERT(0 <= pos && pos < m_size);

    return (((quint8 *)m_map) + m_offset + pos);
}

VOLCANO_INLINE Heap &Buffer::heap(void)
{
    return m_heap;
}

VOLCANO_INLINE const Heap &Buffer::heap(void) const
{
    return m_heap;
}

VOLCANO_GRAPHICS_OPENGL_END

#endif // VOLCANO_GRAPHICS_OPENGL_BUFFER_HPP
