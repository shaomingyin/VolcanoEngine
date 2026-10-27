#ifndef VOLCANO_GRAPHICS_LIGHTINGPASS_H
#define VOLCANO_GRAPHICS_LIGHTINGPASS_H

#include <cstddef>

#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Pass.h>

VOLCANO_GRAPHICS_BEGIN

class GBuffer;

class LightingPass final : public Pass {
public:
    explicit LightingPass(std::size_t maxLights = 16);

    void render(const View& view, const GBuffer& geometry);

private:
    static constexpr GLuint ColorOutput = 0;

private:
    static constexpr GLuint AlbedoTexture = 0;
    static constexpr GLuint NormalTexture = 1;
    static constexpr GLuint PositionTexture = 2;
    static constexpr GLuint DepthTexture = 3;

private:
    LightingUniforms uniforms_;
    GLint albedo_sampler_ = -1;
    GLint normal_sampler_ = -1;
    GLint position_sampler_ = -1;
    GLint depth_sampler_ = -1;
    gl::VertexArray vertex_array_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_LIGHTINGPASS_H
