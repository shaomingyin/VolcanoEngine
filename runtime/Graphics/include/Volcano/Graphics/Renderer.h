#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <cstddef>
#include <memory>

#include <GL/gl3w.h>

#include <Volcano/Graphics/GeometryPass.h>
#include <Volcano/Graphics/GBuffer.h>
#include <Volcano/Graphics/LightingPass.h>
#include <Volcano/Graphics/TransparentPass.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Target.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer final {
public:
    // The caller must activate the OpenGL context before construction.
    explicit Renderer(GL3WGetProcAddressProc resolver);
    ~Renderer();
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    std::size_t maxLights() const noexcept {
        return max_lights_;
    }

    // Rebuilds both lighting programs; failure preserves the current capacity and passes.
    void setMaxLights(std::size_t maxLights);

    // Requires a completed View submission; clears the target and executes the rendering passes.
    void render(const View& view, Target& target);

    const GBuffer& geometryBuffer() const noexcept {
        return *geometry_buffer_;
    }

private:
    void release() noexcept;
    void bind(Target& target) noexcept;
    void clear(const Colorf& color);

private:
    GL3WProcs procs_{};
    std::size_t max_lights_ = 0;
    std::unique_ptr<GBuffer> geometry_buffer_;
    std::unique_ptr<GeometryPass> geometry_pass_;
    std::unique_ptr<LightingPass> lighting_pass_;
    std::unique_ptr<TransparentPass> transparent_pass_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
