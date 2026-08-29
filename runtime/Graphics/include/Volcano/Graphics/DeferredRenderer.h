#ifndef VOLCANO_GRAPHICS_DEFERREDRENDERER_H
#define VOLCANO_GRAPHICS_DEFERREDRENDERER_H

#include <Volcano/Graphics/GeometryPass.h>
#include <Volcano/Graphics/LightingPass.h>
#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

class DeferredRenderer final : public Renderer {
public:
    explicit DeferredRenderer(World::Scene& scene);
    ~DeferredRenderer() override = default;

    DeferredRenderer(const DeferredRenderer&) = delete;
    DeferredRenderer(DeferredRenderer&&) = delete;

public:
    DeferredRenderer& operator=(const DeferredRenderer&) = delete;
    DeferredRenderer& operator=(DeferredRenderer&&) = delete;

    void build() noexcept override;
    void resize(int width, int height) noexcept override;
    void render() override;

private:
    GBuffer gbuffer_;
    GeometryPass geometry_pass_;
    LightingPass lighting_pass_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_DEFERREDRENDERER_H
