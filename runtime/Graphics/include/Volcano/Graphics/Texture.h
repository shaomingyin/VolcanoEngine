#ifndef VOLCANO_GRAPHICS_TEXTURE_H
#define VOLCANO_GRAPHICS_TEXTURE_H

#include <GL/gl.hpp>
#include <Volcano/Graphics/Target.h>
#include <span>

VOLCANO_GRAPHICS_BEGIN

// Uploads straight RGBA8, bottom row first; stores premultiplied alpha internally.
// Creation, use and destruction require the resource's OpenGL context to be current.
class Texture final : public Target {
public:
    Texture(int width, int height, std::span<const std::uint8_t> rgba = {});
    ~Texture() override = default;

    void resize(int width, int height);
    void upload(std::span<const std::uint8_t> rgba);

    int width() const noexcept override { return width_; }
    int height() const noexcept override { return height_; }

private:
    friend class Renderer;
    std::uint32_t framebuffer() const noexcept override { return framebuffer_.id(); }
    void allocate(int width, int height);
    gl::Texture2D texture_;
    gl::Renderbuffer depth_;
    gl::Framebuffer framebuffer_;
    int width_ = 0;
    int height_ = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_TEXTURE_H
