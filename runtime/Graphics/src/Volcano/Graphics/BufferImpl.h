//
//
#ifndef VOLCANO_GRAPHICS_BUFFERIMPL_H
#define VOLCANO_GRAPHICS_BUFFERIMPL_H

#include <functional>

#include <QOpenGLBuffer>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Buffer.h>

VOLCANO_GRAPHICS_BEGIN

class BufferImpl: public Buffer {
public:
    using FreeFunction = std::function<void (GLsizei, GLsizei)>;

public:
    BufferImpl(QOpenGLBuffer& owner, GLsizei offset, GLsizei size, FreeFunction free_fn);
    ~BufferImpl() override;

public:
    GLsizei offset() const {
        return offset_;
    }

    bool open(QIODeviceBase::OpenMode mode) override;
    qint64 size() const override;
    bool bind() override;
    void *map(OpenMode mode) override;
    void *mapRange(int offset, int count, OpenMode mode) override;
    void unmap() override;

protected:
    qint64 readData(char* data, qint64 max_size) override;
    qint64 writeData(const char* data, qint64 max_size) override;

private:
    bool isCurrent() const {
        return (glCurrentBuffer(owner_.type()) == owner_.bufferId());
    }

private:
    QOpenGLBuffer& owner_;
    GLsizei offset_;
    GLsizei size_;
    FreeFunction free_fn_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_BUFFERIMPL_H
