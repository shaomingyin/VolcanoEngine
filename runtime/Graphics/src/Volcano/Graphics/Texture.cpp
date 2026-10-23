#include <Volcano/Graphics/Texture.h>
#include <algorithm>
#include <vector>

VOLCANO_GRAPHICS_BEGIN

Texture::Texture(int width, int height, std::span<const std::uint8_t> rgba) {
    allocate(width, height);
    if (!rgba.empty()) {
        upload(rgba);
    }
}

void Texture::allocate(int width, int height) {
    GLint maximum = 0, maximum_depth = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maximum);
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maximum_depth);
    if (width <= 0 || height <= 0 || width > std::min(maximum, maximum_depth) || height > std::min(maximum, maximum_depth)) {
        throw std::invalid_argument("Texture dimensions exceed OpenGL limits");
    }
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    gl::Texture2D texture;
    gl::Renderbuffer depth;
    gl::Framebuffer framebuffer;
    texture.parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture.parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture.parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture.parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    texture.image2D(0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    depth.storage(GL_DEPTH24_STENCIL8, width, height);
    framebuffer.attachTexture2D(GL_COLOR_ATTACHMENT0, texture);
    framebuffer.attachRenderbuffer(GL_DEPTH_STENCIL_ATTACHMENT, depth);
    framebuffer.checkComplete();
    texture_ = std::move(texture);
    depth_ = std::move(depth);
    framebuffer_ = std::move(framebuffer);
    width_ = width;
    height_ = height;
}

void Texture::resize(int width, int height) {
    if (width != width_ || height != height_) {
        allocate(width, height);
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
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    texture_.subImage2D(0, 0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
}

VOLCANO_GRAPHICS_END
