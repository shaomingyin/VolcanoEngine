#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <optional>
#include <Volcano/Graphics/GeometryPass.h>
#include <Volcano/Graphics/LightingPass.h>
#include <Volcano/Graphics/TransparentPass.h>
#include <Volcano/Graphics/RenderContext.h>
#include <Volcano/Graphics/Material.h>
#include <Volcano/Graphics/Mesh.h>
#include <Volcano/Graphics/Target.h>
#include <Volcano/Math/Math.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer final {
public:
    // The caller must activate the Context before construction.
    explicit Renderer(GL3WGetProcAddressProc resolver);
    ~Renderer();
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    // Multi-pass entry point; execution is not implemented yet.
    void render(const RenderContext& context);

    void begin(Target& target, const Math::Matrix4f& projection = Math::Matrix4f(), const Math::Matrix4f& view = Math::Matrix4f());
    void clear(const Math::Colorf& color = Math::Colorf::Black);
    void draw(const Mesh& mesh, const Material& material = {}, const Math::Affine3f& transform = {});
    // The quad occupies [0, 1] in local X/Y, with its origin at the bottom left.
    void draw(const Texture& texture, const Math::Affine3f& transform = {}, const Math::Colorf& color = Math::Colorf::White);

    void setDepthTest(bool enabled) noexcept { depth_test_ = enabled; }

private:
    GL3WProcs functions_{};

    GeometryPass geometry_pass_;
    LightingPass lighting_pass_;
    TransparentPass transparent_pass_;

    void bind();
    struct Resources {
        Resources();
        gl::Program program;
        Mesh quad;
    };
    std::optional<Resources> resources_;
    Target* target_ = nullptr;
    Math::Matrix4f projection_;
    Math::Matrix4f view_;
    bool depth_test_ = true;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
