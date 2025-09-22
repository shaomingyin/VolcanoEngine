//
//
#include <QThread>
#include <QScopeGuard>
#include <QOpenGLContext>

#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(QObject* parent)
    : Context(parent)
    , gl_context_(QOpenGLContext::currentContext())
    , static_vertex_buffer_allocator_(QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::StaticDraw)
    , static_index_buffer_allocator_(QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::StaticDraw) {
    Q_ASSERT(gl_context_ != nullptr);
    Q_ASSERT(gl_context_->thread()->isCurrentThread());
    Q_ASSERT(gl_context_->thread() == thread());
}

void Renderer::render(const View& view) {
    Q_ASSERT(thread()->isCurrentThread());
    Q_ASSERT(QOpenGLContext::currentContext() != nullptr);
    Q_ASSERT(QOpenGLContext::currentContext()->thread()->isCurrentThread());
}

Buffer* Renderer::allocStaticVertexBuffer(GLsizei size) {
    Q_ASSERT(thread()->isCurrentThread());
    return static_vertex_buffer_allocator_.allocBuffer(size);
}

Buffer* Renderer::allocStaticIndexBuffer(GLsizei size) {
    Q_ASSERT(thread()->isCurrentThread());
    return static_index_buffer_allocator_.allocBuffer(size);
}

VOLCANO_GRAPHICS_END
