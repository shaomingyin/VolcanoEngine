#include <stdexcept>
#include <utility>
#include <vector>

#include <GL/gl.hpp>

#include <Volcano/Graphics/Texture.h>

VOLCANO_GRAPHICS_BEGIN

Texture::Texture(Texture&&) noexcept = default;
Texture& Texture::operator=(Texture&&) noexcept = default;

const gl::Texture2D& Texture::texture() const noexcept {
    return texture_;
}

Texture::Texture(int width, int height, std::span<const std::uint8_t> rgba)
    : texture_(allocate(width, height))
    , width_(width)
    , height_(height) {
    if (!rgba.empty()) {
        upload(rgba);
    }
}

gl::Texture2D Texture::allocate(int width, int height) {
    GLint maximum = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maximum);
    if (width <= 0 || height <= 0 || width > maximum || height > maximum) {
        throw std::invalid_argument("Texture dimensions exceed OpenGL limits");
    }

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    gl::Texture2D texture;
    texture.parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture.parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture.parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture.parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    texture.image2D(0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    return texture;
}

void Texture::resize(int width, int height) {
    if (width != width_ || height != height_) {
        texture_ = allocate(width, height);
        width_ = width;
        height_ = height;
    }
}

void Texture::upload(std::span<const std::uint8_t> rgba) {
    if (rgba.size() != static_cast<std::size_t>(width_) * static_cast<std::size_t>(height_) * 4) {
        throw std::invalid_argument("Texture upload requires width * height * 4 RGBA bytes");
    }

    // Store premultiplied pixels so rendered textures can be composited again.
    std::vector<std::uint8_t> pixels(rgba.begin(), rgba.end());
    for (std::size_t i = 0; i < pixels.size(); i += 4) {
        for (std::size_t channel = 0; channel < 3; ++channel) {
            pixels[i + channel] = static_cast<std::uint8_t>((pixels[i + channel] * pixels[i + 3] + 127) / 255);
        }
    }

    uploadPremultiplied(pixels);
}

void Texture::uploadPremultiplied(std::span<const std::uint8_t> rgba) {
    if (rgba.size() != static_cast<std::size_t>(width_) * static_cast<std::size_t>(height_) * 4) {
        throw std::invalid_argument("Texture upload requires width * height * 4 RGBA bytes");
    }

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    texture_.subImage2D(0, 0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
}

VOLCANO_GRAPHICS_END
