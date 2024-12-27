//
//
#include <QScopeGuard>
#include <QOpenGLContext>

#include <Volcano/Graphics/BufferAllocator.h>

#include "BufferImpl.h"

VOLCANO_GRAPHICS_BEGIN

BufferAllocator::Block::Block(Type type, GLsizei size)
    : QOpenGLBuffer(type)
    , size_(size)
    , allocator_(buddy2_new(size)) {
}

BufferAllocator::Block::~Block() {
    buddy2_destroy(allocator_);
}

Buffer* BufferAllocator::Block::allocBuffer(GLsizei size) {
    int offset = buddy2_alloc(allocator_, size);
    if (offset >= 0) {
        return new BufferImpl(*this, offset, size, [this](GLsizei offset, GLsizei size) {
            buddy2_free(allocator_, offset);
        });
    }
    return nullptr;
}

BufferAllocator::BufferAllocator(QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage, GLsizei block_size)
    : type_(type)
    , usage_(usage)
    , block_size_(block_size) {
}

BufferAllocator::~BufferAllocator() {
}

Buffer* BufferAllocator::allocBuffer(GLsizei size) {
    Q_ASSERT(QOpenGLContext::currentContext() != nullptr);

    if (size > block_size_) {
        return nullptr;
    }

    Buffer* p = nullptr;

    for (auto& block: blocks_) {
        p = block.allocBuffer(size);
        if (p != nullptr) {
            return p;
        }
    }

    auto& block = blocks_.emplaceBack(type_, block_size_);
    auto block_guard = QScopeGuard([this] {
        blocks_.removeLast();
    });

    if (!block.create()) {
        return nullptr;
    }

    block.allocate(block_size_);
    p = block.allocBuffer(size);
    if (p == nullptr) {
        return nullptr;
    }

    block_guard.dismiss();

    return p;
}

VOLCANO_GRAPHICS_END
