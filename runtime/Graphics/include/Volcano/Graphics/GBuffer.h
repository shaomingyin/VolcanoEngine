#ifndef VOLCANO_GRAPHICS_GBUFFER_H
#define VOLCANO_GRAPHICS_GBUFFER_H

#include <GL/gl.hpp>

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class GBuffer final {
public:
    GBuffer();
    ~GBuffer() = default;

    GBuffer(const GBuffer&) = delete;
    GBuffer(GBuffer&&) noexcept = default;

public:
    GBuffer& operator=(const GBuffer&) = delete;
    GBuffer& operator=(GBuffer&&) noexcept = default;

    void resize(int width, int height) noexcept;

    void bindForGeometry() const noexcept;
    void bindForLighting() const noexcept;

    const gl::Framebuffer& framebuffer() const noexcept {
        return framebuffer_;
    }

    const gl::Texture2D& position() const noexcept {
        return position_;
    }

    const gl::Texture2D& normal() const noexcept {
        return normal_;
    }

    const gl::Texture2D& albedoSpecular() const noexcept {
        return albedo_specular_;
    }

    const gl::Texture2D& depth() const noexcept {
        return depth_;
    }

    int width() const noexcept {
        return width_;
    }

    int height() const noexcept {
        return height_;
    }

    bool complete() const noexcept;

private:
    gl::Framebuffer framebuffer_;
    gl::Texture2D position_;
    gl::Texture2D normal_;
    gl::Texture2D albedo_specular_;
    gl::Texture2D depth_;
    int width_ = 0;
    int height_ = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_GBUFFER_H
