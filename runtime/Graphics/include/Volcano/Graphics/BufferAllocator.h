//
//
#ifndef VOLCANO_GRAPHICS_BUFFERALLOCATOR_H
#define VOLCANO_GRAPHICS_BUFFERALLOCATOR_H

#include <QList>
#include <QOpenGLBuffer>

extern "C" {
#include <buddy2.h>
}

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Buffer.h>

VOLCANO_GRAPHICS_BEGIN

class BufferAllocator {
public:
    BufferAllocator(QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage, GLsizei block_size = 64 * 1024 * 1024);
    virtual ~BufferAllocator();

public:
    QOpenGLBuffer::Type type() const {
        return type_;
    }

    QOpenGLBuffer::UsagePattern usage() const {
        return usage_;
    }

    GLsizei blockSize() const {
        return block_size_;
    }

    Buffer* allocBuffer(GLsizei size);

private:
    class Block final: public QOpenGLBuffer {
    public:
        Block(Type type, GLsizei size);
        ~Block();

    public:
        Buffer* allocBuffer(GLsizei size);

    private:
        GLsizei size_;
        buddy2* allocator_;
    };

    QOpenGLBuffer::Type type_;
    QOpenGLBuffer::UsagePattern usage_;
    GLsizei block_size_;
    QList<Block> blocks_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_BUFFERALLOCATOR_H
