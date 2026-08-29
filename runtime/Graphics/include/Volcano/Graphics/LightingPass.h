#ifndef VOLCANO_GRAPHICS_LIGHTINGPASS_H
#define VOLCANO_GRAPHICS_LIGHTINGPASS_H

#include <GL/gl.hpp>

#include <Volcano/Graphics/GBuffer.h>
#include <Volcano/Graphics/Pass.h>

VOLCANO_GRAPHICS_BEGIN

class LightingPass final : public Pass {
public:
    explicit LightingPass(GBuffer& gbuffer);
    ~LightingPass() override = default;

    LightingPass(const LightingPass&) = delete;
    LightingPass(LightingPass&&) = delete;

public:
    LightingPass& operator=(const LightingPass&) = delete;
    LightingPass& operator=(LightingPass&&) = delete;

    void resize(int width, int height) noexcept override;
    void execute(const Frame& frame) override;

private:
    GBuffer& gbuffer_;
    gl::Program program_;
    gl::VertexArray vertex_array_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_LIGHTINGPASS_H
