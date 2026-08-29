#include <Volcano/Graphics/GBuffer.h>

VOLCANO_GRAPHICS_BEGIN

namespace {

void configureColorTexture(const gl::Texture2D& texture, GLint internal_format, GLenum type, GLsizei width, GLsizei height) noexcept {
    texture.parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    texture.parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    texture.parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture.parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    texture.image2D(0, internal_format, width, height, 0, GL_RGBA, type, nullptr);
}

void configureDepthTexture(const gl::Texture2D& texture, GLsizei width, GLsizei height) noexcept {
    texture.parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    texture.parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    texture.parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture.parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    texture.image2D(0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
}

}

GBuffer::GBuffer() {
}

void GBuffer::resize(int width, int height) noexcept {
    if (width <= 0 || height <= 0) {
        width_ = 0;
        height_ = 0;
        return;
    }

    width_ = width;
    height_ = height;

    configureColorTexture(position_, GL_RGBA16F, GL_FLOAT, width, height);
    configureColorTexture(normal_, GL_RGBA16F, GL_FLOAT, width, height);
    configureColorTexture(albedo_specular_, GL_RGBA8, GL_UNSIGNED_BYTE, width, height);
    configureDepthTexture(depth_, width, height);

    framebuffer_.attachTexture2D(GL_COLOR_ATTACHMENT0, position_);
    framebuffer_.attachTexture2D(GL_COLOR_ATTACHMENT1, normal_);
    framebuffer_.attachTexture2D(GL_COLOR_ATTACHMENT2, albedo_specular_);
    framebuffer_.attachTexture2D(GL_DEPTH_ATTACHMENT, depth_);

    const GLenum draw_buffers[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2
    };
    framebuffer_.drawBuffers(draw_buffers, 3);

    if (!complete()) {
        spdlog::error("OpenGL G-buffer is incomplete.");
    }
}

void GBuffer::bindForGeometry() const noexcept {
    framebuffer_.bind();
}

void GBuffer::bindForLighting() const noexcept {
    position_.bind(0);
    normal_.bind(1);
    albedo_specular_.bind(2);
    depth_.bind(3);
}

bool GBuffer::complete() const noexcept {
    return framebuffer_.complete();
}

VOLCANO_GRAPHICS_END
