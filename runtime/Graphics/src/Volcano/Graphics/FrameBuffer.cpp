#include <stdexcept>
#include <utility>

#include <GL/gl.hpp>

#include <Volcano/Graphics/FrameBuffer.h>

VOLCANO_GRAPHICS_BEGIN

void FrameBuffer::bind() const noexcept {
    framebuffer_.bind();
}

FrameBuffer::FrameBuffer(int width, int height)
    : texture_(width, height) {
    GLint maximum = 0;
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maximum);
    if (width > maximum || height > maximum) {
        throw std::invalid_argument("FrameBuffer dimensions exceed OpenGL limits");
    }

    depth_.storage(GL_DEPTH24_STENCIL8, width, height);
    framebuffer_.attachTexture2D(GL_COLOR_ATTACHMENT0, texture_.texture());
    framebuffer_.attachRenderbuffer(GL_DEPTH_STENCIL_ATTACHMENT, depth_);
    framebuffer_.checkComplete();
}

void FrameBuffer::resize(int width, int height) {
    if (width == this->width() && height == this->height()) {
        return;
    }

    FrameBuffer replacement(width, height);
    std::swap(texture_, replacement.texture_);
    std::swap(depth_, replacement.depth_);
    std::swap(framebuffer_, replacement.framebuffer_);
}

VOLCANO_GRAPHICS_END
