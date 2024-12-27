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

    GLsizei size() const override;
    void bind() override;
    void *map(QOpenGLBuffer::RangeAccessFlag access) override;
    void *mapRange(int offset, int count, QOpenGLBuffer::RangeAccessFlags access) override;
    void unmap() override;

private:
    QOpenGLBuffer& owner_;
    GLsizei offset_;
    GLsizei size_;
    FreeFunction free_fn_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_BUFFERIMPL_H
