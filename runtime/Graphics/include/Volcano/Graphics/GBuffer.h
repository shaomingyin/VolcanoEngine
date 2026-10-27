#ifndef VOLCANO_GRAPHICS_GBUFFER_H
#define VOLCANO_GRAPHICS_GBUFFER_H

#include <string>

#include <GL/gl.hpp>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Size.h>

VOLCANO_GRAPHICS_BEGIN

// Intermediate geometry data; creation, use and destruction require the current OpenGL context.
class GBuffer final {
public:
    GBuffer() = default;

    void resize(Size size);
    Size size() const noexcept {
        return size_;
    }

    const gl::Framebuffer& framebuffer() const noexcept {
        return framebuffer_;
    }

    const gl::Texture2D& albedo() const noexcept {
        return albedo_;
    }

    // XYZ is the world normal; W selects whether lighting applies.
    const gl::Texture2D& normal() const noexcept {
        return normal_;
    }

    const gl::Texture2D& position() const noexcept {
        return position_;
    }

    const gl::Texture2D& depth() const noexcept {
        return depth_;
    }

    static std::string shaderDefinitions();

public:
    static constexpr GLuint AlbedoAttachment = 0;
    static constexpr GLuint NormalAttachment = 1;
    static constexpr GLuint PositionAttachment = 2;

private:
    gl::Texture2D albedo_;
    gl::Texture2D normal_;
    gl::Texture2D position_;
    gl::Texture2D depth_;
    gl::Framebuffer framebuffer_;
    Size size_{};
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_GBUFFER_H
