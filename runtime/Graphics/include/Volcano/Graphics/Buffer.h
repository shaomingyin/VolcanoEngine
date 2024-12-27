//
//
#ifndef VOLCANO_GRAPHICS_BUFFER_H
#define VOLCANO_GRAPHICS_BUFFER_H

#include <QOpenGLBuffer>

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Buffer {
public:
    Buffer() = default;
    virtual ~Buffer() = default;

public:
    virtual GLsizei size() const = 0;
    virtual void bind() = 0;
    virtual void *map(QOpenGLBuffer::RangeAccessFlag access) = 0;
    virtual void *mapRange(int offset, int count, QOpenGLBuffer::RangeAccessFlags access) = 0;
    virtual void unmap() = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_BUFFER_H
