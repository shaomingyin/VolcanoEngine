#include <limits>
#include <stdexcept>
#include <utility>

#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Graphics/Texture.h>
#include <Volcano/ScopeGuard.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(GL3WGetProcAddressProc resolver) {
    auto* previous = gl3wProcs;
    auto rollback = scopeGuard([this, previous]() noexcept {
        release();
        gl3wProcs = previous;
    });

    if (resolver == nullptr) {
        throw std::invalid_argument("OpenGL function resolver must not be null");
    }
    if (gl3wInit(&procs_, resolver) != GL3W_OK) {
        throw std::runtime_error("Cannot load OpenGL functions from the current context");
    }

    GLint major = 0, minor = 0;
    procs_.gl.GetIntegerv(GL_MAJOR_VERSION, &major);
    procs_.gl.GetIntegerv(GL_MINOR_VERSION, &minor);
    if (major < 3 || (major == 3 && minor < 3)) {
        throw std::runtime_error("Graphics requires OpenGL 3.3 or newer");
    }

    gl3wProcs = &procs_;
    geometry_buffer_ = std::make_unique<GBuffer>();
    geometry_pass_ = std::make_unique<GeometryPass>();
    setMaxLights(16);
    rollback.dismiss();
}

Renderer::~Renderer() {
    auto* previous = gl3wProcs == &procs_ ? nullptr : gl3wProcs;
    release();
    gl3wProcs = previous;
}

void Renderer::release() noexcept {
    gl3wProcs = &procs_;
    transparent_pass_.reset();
    lighting_pass_.reset();
    geometry_pass_.reset();
    geometry_buffer_.reset();
}

void Renderer::setMaxLights(std::size_t maxLights) {
    if (maxLights == 0 || maxLights > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        throw std::invalid_argument("Light capacity must be a positive GLSL int");
    }
    if (maxLights == max_lights_) {
        return;
    }

    auto* previous = gl3wProcs;
    auto restore = scopeGuard([previous]() noexcept {
        gl3wProcs = previous;
    });
    gl3wProcs = &procs_;

    auto lighting = std::make_unique<LightingPass>(maxLights);
    auto transparent = std::make_unique<TransparentPass>(maxLights);
    lighting_pass_ = std::move(lighting);
    transparent_pass_ = std::move(transparent);
    max_lights_ = maxLights;
}

void Renderer::render(const View& view, Target& target) {
    if (!view.ready()) {
        throw std::logic_error("View rendering requires a successful begin()/end() submission");
    }
    if (view.lights().size() > max_lights_) {
        throw std::invalid_argument("View exceeds the renderer's light limit");
    }
    for (const auto& item : view.items()) {
        if (item.material.texture != nullptr && target.contains(*item.material.texture)) {
            throw std::invalid_argument("Cannot sample a Texture while rendering to its FrameBuffer");
        }
    }
    if (target.width() <= 0 || target.height() <= 0) {
        return;
    }

    auto state_guard = scopeGuard([this, &target]() noexcept {
        bind(target);
    });

    gl3wProcs = &procs_;
    geometry_buffer_->resize(target.size());
    bind(target);
    clear(view.clearColor());

    geometry_buffer_->framebuffer().bind();
    constexpr GLfloat empty[] = {0, 0, 0, 0};
    constexpr GLfloat far_depth = 1;
    for (GLuint attachment : {GBuffer::AlbedoAttachment, GBuffer::NormalAttachment,
                              GBuffer::PositionAttachment}) {
        glClearBufferfv(GL_COLOR, attachment, empty);
    }

    glClearBufferfv(GL_DEPTH, 0, &far_depth);

    geometry_pass_->render(view);

    bind(target);
    lighting_pass_->render(view, *geometry_buffer_);
    transparent_pass_->render(view);
}

void Renderer::bind(Target& target) noexcept {
    gl3wProcs = &procs_;
    target.bind();
    glViewport(0, 0, target.width(), target.height());

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
