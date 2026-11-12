#ifndef VOLCANO_GRAPHICS_TEXTURE_H
#define VOLCANO_GRAPHICS_TEXTURE_H

#include <cstdint>
#include <span>

#include <GL/gl.hpp>

#include <Volcano/Size.h>
#include <Volcano/World/Texture.h>
#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

// Uploads straight RGBA8, bottom row first; stores premultiplied alpha internally.
// Creation, use and destruction require the resource's OpenGL context to be current.
class Texture final : public World::Texture {
public:
    Texture(int width, int height, std::span<const std::uint8_t> rgba = {});
    ~Texture() override = default;
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&&) noexcept;
    Texture& operator=(Texture&&) noexcept;

    const gl::Texture2D& texture() const noexcept;
    void resize(int width, int height);
    void upload(std::span<const std::uint8_t> rgba);
    // Uploads RGBA8 that already uses premultiplied alpha.
    void uploadPremultiplied(std::span<const std::uint8_t> rgba);

    int width() const noexcept {
        return width_;
    }

    int height() const noexcept {
        return height_;
    }

    Size size() const noexcept override {
        return {width_, height_};
    }

private:
    static gl::Texture2D allocate(int width, int height);

private:
    gl::Texture2D texture_;
    int width_ = 0;
    int height_ = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_TEXTURE_H
