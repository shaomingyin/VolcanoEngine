#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <cstddef>
#include <string_view>

#include <Volcano/World/Canvas.h>
#include <Volcano/Graphics/GBuffer.h>
#include <Volcano/Graphics/GeometryPass.h>
#include <Volcano/Graphics/LightingPass.h>
#include <Volcano/Graphics/TransparentPass.h>
#include <Volcano/Graphics/PaintPass.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Target.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer final {
public:
    using FontCacheStats = PaintPass::FontCacheStats;

    Renderer() = default;
    ~Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    const GBuffer& geometryBuffer() const noexcept;
    std::size_t maxLights() const noexcept;
    void setMaxLights(std::size_t count);
    void setFontAtlasBudget(std::size_t bytes);
    std::size_t fontAtlasBytes() const noexcept;
    FontCacheStats fontCacheStats() const noexcept;
    void prewarm(const Font& font, std::string_view text, unsigned int pixels);
    void setFontResolver(PaintPass::FontResolver resolver) {
        paint_pass_.setFontResolver(std::move(resolver));
    }
    void setTextureResolver(PaintPass::TextureResolver resolver) {
        paint_pass_.setTextureResolver(std::move(resolver));
    }
    void render(const View& view, Target& target);
    void renderVG(const World::Canvas& commands, Target& target, bool clear = true);

private:
    void bind(Target& target) noexcept;
    void clear(const Colorf& color);

private:
    std::size_t max_lights_ = 16;
    GBuffer geometry_buffer_;
    GeometryPass geometry_pass_;
    LightingPass lighting_pass_;
    TransparentPass transparent_pass_;
    PaintPass paint_pass_;
};

VOLCANO_GRAPHICS_END

#endif
