#ifndef VOLCANO_GRAPHICS_FRAMEBUFFER_H
#define VOLCANO_GRAPHICS_FRAMEBUFFER_H

#include <GL/gl.hpp>

#include <Volcano/Graphics/Target.h>
#include <Volcano/Graphics/Texture.h>

VOLCANO_GRAPHICS_BEGIN

// Owns one RGBA8 color texture and a depth/stencil attachment.
class FrameBuffer final : public Target {
public:
    FrameBuffer(int width, int height);
    ~FrameBuffer() override = default;

    void bind() const noexcept override;
    void resize(int width, int height);

    int width() const noexcept override {
        return texture_.width();
    }

    int height() const noexcept override {
        return texture_.height();
    }

    const Texture& texture() const noexcept {
        return texture_;
    }

    bool contains(const Texture& texture) const noexcept override {
        return &texture == &texture_;
    }

private:
    Texture texture_;
    gl::Renderbuffer depth_;
    gl::Framebuffer framebuffer_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_FRAMEBUFFER_H
