//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Context.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Pipeline.h>
#include <Volcano/Graphics/BufferAllocator.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer: public Context {
    Q_OBJECT

public:
    Renderer(QObject* parent = nullptr);

public:
    void render(const View& view);
    Buffer* allocStaticVertexBuffer(GLsizei size) override;
    Buffer* allocStaticIndexBuffer(GLsizei size) override;

private:
    QOpenGLContext* gl_context_;
    Pipeline forward_pipeline_;
    Pipeline deferred_pipeline_;
    BufferAllocator static_vertex_buffer_allocator_;
    BufferAllocator static_index_buffer_allocator_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
