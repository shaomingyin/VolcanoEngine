#ifndef VOLCANO_GRAPHICS_TRANSPARENTPASS_H
#define VOLCANO_GRAPHICS_TRANSPARENTPASS_H

#include <cstddef>

#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Pass.h>

VOLCANO_GRAPHICS_BEGIN

class TransparentPass final : public Pass {
public:
    explicit TransparentPass(std::size_t maxLights = 16);

    void render(const View& view);

private:
    static constexpr GLuint ColorOutput = 0;

private:
    ObjectUniforms object_uniforms_;
    LightingUniforms lighting_uniforms_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_TRANSPARENTPASS_H
