#ifndef VOLCANO_GRAPHICS_GEOMETRYPASS_H
#define VOLCANO_GRAPHICS_GEOMETRYPASS_H

#include <GL/gl.hpp>

#include <Volcano/Graphics/GBuffer.h>
#include <Volcano/Graphics/Pass.h>

VOLCANO_GRAPHICS_BEGIN

class GeometryPass final : public Pass {
public:
    explicit GeometryPass(GBuffer& gbuffer);
    ~GeometryPass() override = default;

    GeometryPass(const GeometryPass&) = delete;
    GeometryPass(GeometryPass&&) = delete;

public:
    GeometryPass& operator=(const GeometryPass&) = delete;
    GeometryPass& operator=(GeometryPass&&) = delete;

    void resize(int width, int height) noexcept override;
    void execute(const Frame& frame) override;

private:
    GBuffer& gbuffer_;
    gl::Program program_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_GEOMETRYPASS_H
