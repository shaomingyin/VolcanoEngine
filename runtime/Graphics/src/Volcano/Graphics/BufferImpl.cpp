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
    Q_ASSERT(size >= 0);
    Q_ASSERT(offset < owner.size());
    Q_ASSERT((offset + size) < owner.size());
}

BufferImpl::~BufferImpl() {
    if (free_fn_) {
        free_fn_(offset_, size_);
    }
}

void BufferImpl::bind() {
    owner_.bind();
}

void *BufferImpl::map(QOpenGLBuffer::RangeAccessFlag access) {
    if (owner_.isCreated()) {
        return owner_.mapRange(offset_, size_, access);
    }
    return nullptr;
}

void *BufferImpl::mapRange(int offset, int count, QOpenGLBuffer::RangeAccessFlags access) {
    if (((offset + count) <= size_) && owner_.isCreated()) {
        return owner_.mapRange(offset_ + offset, count, access);
    }
    return nullptr;
}

void BufferImpl::unmap() {
    owner_.unmap();
}

GLsizei BufferImpl::size() const {
    return size_;
}

VOLCANO_GRAPHICS_END
