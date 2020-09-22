//
//
#ifndef VOLCANO_OPENGL_BUFFER_HPP
#define VOLCANO_OPENGL_BUFFER_HPP

#include <QIODevice>
#include <QOpenGLBuffer>

#include <Volcano/OpenGL/Common.hpp>

VOLCANO_OPENGL_BEGIN

class VOLCANO_API Buffer: public QIODevice
{
    Q_OBJECT

public:
    std::function<void (void)> freeFunction;
    std::function<void *(QOpenGLBuffer::Access)> mapFunction;
    std::function<void (void)> unmapFunction;

public:
    Buffer(int offset, int size, QObject *parent = nullptr);
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
    virtual void *map(void);
    virtual void unmap(void);

private:
    int m_offset;
    int m_size;
    int m_pos;
    void *m_map;
};

VOLCANO_INLINE int Buffer::offset(void) const
{
    return m_offset;
}

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_BUFFER_HPP
