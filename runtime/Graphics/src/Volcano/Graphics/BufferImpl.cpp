//
//
#include "BufferImpl.h"

VOLCANO_GRAPHICS_BEGIN

BufferImpl::BufferImpl(QOpenGLBuffer& owner, GLsizei offset, GLsizei size, FreeFunction free_fn)
    : owner_(owner)
    , offset_(offset)
    , size_(size)
    , free_fn_(std::move(free_fn)) {
    Q_ASSERT(offset >= 0);
    Q_ASSERT(size > 0);
    Q_ASSERT(offset < owner.size());
    Q_ASSERT((offset + size) < owner.size());
}

BufferImpl::~BufferImpl() {
    if (free_fn_) {
        free_fn_(offset_, size_);
    }
}

bool BufferImpl::open(QIODeviceBase::OpenMode mode) {
    return owner_.bind();
}

qint64 BufferImpl::size() const {
    return size_;
}

bool BufferImpl::bind() {
    return owner_.bind();
}

void *BufferImpl::map(MapMode mode) {
    if (owner_.isCreated()) {
        Q_ASSERT(isCurrent());
        if (mode == ReadOnly) {
            return owner_.mapRange(offset_, size_, QOpenGLBuffer::RangeWrite);
        }
        if (mode == WriteOnly) {
            return owner_.mapRange(offset_, size_, QOpenGLBuffer::RangeWrite);
        }
        if (mode == ReadWrite) {
            return owner_.mapRange(offset_, size_, QOpenGLBuffer::RangeRead | QOpenGLBuffer::RangeWrite);
        }
    }
    return nullptr;
}

void *BufferImpl::mapRange(int offset, int count, MapMode mode) {
    auto pos = offset_ + offset;
    if (((pos + count) <= size_) && owner_.isCreated()) {
        Q_ASSERT(isCurrent());
        if (mode == ReadOnly) {
            return owner_.mapRange(pos, count, QOpenGLBuffer::RangeWrite);
        }
        if (mode == WriteOnly) {
            return owner_.mapRange(pos, count, QOpenGLBuffer::RangeWrite);
        }
        if (mode == ReadWrite) {
            return owner_.mapRange(pos, count, QOpenGLBuffer::RangeRead | QOpenGLBuffer::RangeWrite);
        }
    }
    return nullptr;
}

void BufferImpl::unmap() {
    Q_ASSERT(isCurrent());
    owner_.unmap();
}

qint64 BufferImpl::readData(char* data, qint64 max_size) {
    return 0;
}

qint64 BufferImpl::writeData(const char* data, qint64 max_size) {
    return 0;
}

VOLCANO_GRAPHICS_END
