#include <Volcano/Graphics/DeferredRenderer.h>

VOLCANO_GRAPHICS_BEGIN

DeferredRenderer::DeferredRenderer(World::Scene& scene)
    : Renderer(scene)
    , geometry_pass_(gbuffer_)
    , lighting_pass_(gbuffer_) {
}

void DeferredRenderer::build() noexcept {
    Renderer::build();
    gbuffer_.resize(width(), height());
}

void DeferredRenderer::resize(int width, int height) noexcept {
    Renderer::resize(width, height);
    gbuffer_.resize(width, height);
}

void DeferredRenderer::render() {
    if (gbuffer_.width() != width() || gbuffer_.height() != height()) {
        gbuffer_.resize(width(), height());
    }

    geometry_pass_.execute(frame());
    lighting_pass_.execute(frame());
}

VOLCANO_GRAPHICS_END
