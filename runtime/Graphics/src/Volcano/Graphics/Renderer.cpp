#include <limits>
#include <stdexcept>
#include <utility>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Graphics/GeometryPass.h>
#include <Volcano/Graphics/LightingPass.h>
#include <Volcano/Graphics/TransparentPass.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Graphics/Texture.h>

VOLCANO_GRAPHICS_BEGIN

const GBuffer& Renderer::geometryBuffer() const noexcept {
    return geometry_buffer_;
}

std::size_t Renderer::maxLights() const noexcept {
    return max_lights_;
}

void Renderer::setFontAtlasBudget(std::size_t bytes) {
    paint_pass_.setAtlasBudget(bytes);
}

std::size_t Renderer::fontAtlasBytes() const noexcept {
    return paint_pass_.atlasBytes();
}

Renderer::FontCacheStats Renderer::fontCacheStats() const noexcept {
    return paint_pass_.fontCacheStats();
}

void Renderer::prewarm(const Font& font, std::string_view text, unsigned int pixels) {
    paint_pass_.prewarm(font, text, pixels);
}

void Renderer::renderVG(const World::Canvas& commands, Target& target, bool clear) {
    auto restore = scopeGuard([this, &target] { bind(target); });
    paint_pass_.render(commands, target, clear);
}

void Renderer::setMaxLights(std::size_t maxLights) {
    if (maxLights == 0 || maxLights > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        throw std::invalid_argument("Light capacity must be a positive GLSL int");
    }
    if (maxLights == max_lights_) {
        return;
    }


    LightingPass lighting(maxLights);
    TransparentPass transparent(maxLights);
    lighting_pass_ = std::move(lighting);
    transparent_pass_ = std::move(transparent);
    max_lights_ = maxLights;
}

void Renderer::render(const View& view, Target& target) {
    VOLCANO_ASSERT(view.ready());
    if (view.lights().size() > max_lights_) {
        throw std::invalid_argument("View exceeds the renderer's light limit");
    }
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    for (const auto& item : view.items()) {
        VOLCANO_ASSERT(item.material.texture == nullptr || !target.contains(*item.material.texture));
    }
#endif
    const Size targetSize = target.size();
    if (targetSize.width <= 0 || targetSize.height <= 0) {
        return;
    }

    auto state_guard = scopeGuard([this, &target]() noexcept {
        bind(target);
    });

    for (const auto& task : view.vgTasks()) {
        paint_pass_.render(task.commands.get(), task.target.get(), true);
    }
    geometry_buffer_.resize(targetSize);
    bind(target, targetSize);
    clear(view.clearColor());

    geometry_buffer_.framebuffer().bind();
    constexpr GLfloat empty[] = {0, 0, 0, 0};
    constexpr GLfloat far_depth = 1;
    for (GLuint attachment : {GBuffer::AlbedoAttachment, GBuffer::NormalAttachment,
                              GBuffer::PositionAttachment}) {
        glClearBufferfv(GL_COLOR, attachment, empty);
    }

    glClearBufferfv(GL_DEPTH, 0, &far_depth);

    geometry_pass_.render(view);

    bind(target, targetSize);
    lighting_pass_.render(view, geometry_buffer_);
    transparent_pass_.render(view);
    for (const auto& overlay : view.overlays()) {
        paint_pass_.render(overlay.get(), target, false);
    }
    if (view.hud() != nullptr) {
        paint_pass_.render(*view.hud(), target);
    }
}

void Renderer::bind(Target& target) noexcept {
    bind(target, target.size());
}

void Renderer::bind(Target& target, Size viewportSize) noexcept {
    target.bind();
    glViewport(0, 0, viewportSize.width, viewportSize.height);

    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_FRAMEBUFFER_SRGB);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glDepthRange(0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::clear(const Colorf& color) {
    glClearColor(color.red() * color.alpha(), color.green() * color.alpha(), color.blue() * color.alpha(), color.alpha());
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

VOLCANO_GRAPHICS_END
