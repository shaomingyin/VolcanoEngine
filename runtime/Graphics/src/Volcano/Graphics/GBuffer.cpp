#include <stdexcept>
#include <utility>

#include <Volcano/Graphics/GBuffer.h>

VOLCANO_GRAPHICS_BEGIN

std::string GBuffer::shaderDefinitions() {
    return "#define ALBEDO_ATTACHMENT " + std::to_string(AlbedoAttachment) + "\n"
        + "#define NORMAL_ATTACHMENT " + std::to_string(NormalAttachment) + "\n"
        + "#define POSITION_ATTACHMENT " + std::to_string(PositionAttachment) + "\n";
}

void GBuffer::resize(Size size) {
    if (size == size_ && !size_.empty()) {
        return;
    }

    GLint maximum = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maximum);
    if (size.width <= 0 || size.height <= 0 || size.width > maximum || size.height > maximum) {
        throw std::invalid_argument("GBuffer dimensions exceed OpenGL limits");
    }

    gl::Texture2D albedo, normal, position, depth;
    gl::Framebuffer framebuffer;
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    for (const auto* texture : {&albedo, &normal, &position, &depth}) {
        texture->parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        texture->parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        texture->parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        texture->parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    for (const auto* texture : {&albedo, &normal, &position}) {
        texture->image2D(0, GL_RGBA32F, size.width, size.height, 0, GL_RGBA, GL_FLOAT, nullptr);
    }

    depth.image2D(0, GL_DEPTH_COMPONENT24, size.width, size.height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);
    framebuffer.attachTexture2D(GL_COLOR_ATTACHMENT0 + GBuffer::AlbedoAttachment, albedo);
    framebuffer.attachTexture2D(GL_COLOR_ATTACHMENT0 + GBuffer::NormalAttachment, normal);
    framebuffer.attachTexture2D(GL_COLOR_ATTACHMENT0 + GBuffer::PositionAttachment, position);
    framebuffer.attachTexture2D(GL_DEPTH_ATTACHMENT, depth);
    constexpr GLenum buffers[] = {GL_COLOR_ATTACHMENT0 + GBuffer::AlbedoAttachment,
        GL_COLOR_ATTACHMENT0 + GBuffer::NormalAttachment,
        GL_COLOR_ATTACHMENT0 + GBuffer::PositionAttachment};
    framebuffer.drawBuffers(buffers, 3);
    framebuffer.checkComplete();

    albedo_ = std::move(albedo);
    normal_ = std::move(normal);
    position_ = std::move(position);
    depth_ = std::move(depth);
    framebuffer_ = std::move(framebuffer);
    size_ = size;
}

VOLCANO_GRAPHICS_END
