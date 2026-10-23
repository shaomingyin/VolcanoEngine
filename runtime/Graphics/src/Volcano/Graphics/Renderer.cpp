#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Graphics/Texture.h>

VOLCANO_GRAPHICS_BEGIN

namespace {

constexpr Mesh::Vertex QuadVertices[] = {
    {{0, 0, 0}, {0, 0, 1}, {0, 0}},
    {{1, 0, 0}, {0, 0, 1}, {1, 0}},
    {{1, 1, 0}, {0, 0, 1}, {1, 1}},
    {{0, 1, 0}, {0, 0, 1}, {0, 1}}
};
constexpr Mesh::index_type QuadIndices[] = {0, 1, 2, 0, 2, 3};

constexpr const char* VertexSource = R"GLSL(
#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 2) in vec2 aTexcoord;
uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;
out vec2 vTexcoord;
void main() {
    vTexcoord = aTexcoord;
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}
)GLSL";

constexpr const char* FragmentSource = R"GLSL(
#version 330 core
uniform vec4 uColor;
uniform sampler2D uTexture;
uniform bool uTextured;
in vec2 vTexcoord;
out vec4 color;
void main() {
    color = vec4(uColor.rgb * uColor.a, uColor.a);
    if (uTextured) color *= texture(uTexture, vTexcoord);
    if (color.a == 0.0) discard;
}
)GLSL";

}

Renderer::Resources::Resources()
    : quad(QuadVertices, QuadIndices) {
    program.attachSource(GL_VERTEX_SHADER, VertexSource);
    program.attachSource(GL_FRAGMENT_SHADER, FragmentSource);
    program.link();
}

Renderer::Renderer(GL3WGetProcAddressProc resolver) {
    if (resolver == nullptr) {
        throw std::invalid_argument("OpenGL function resolver must not be null");
    }
    if (gl3wInit(&functions_, resolver) != GL3W_OK) {
        throw std::runtime_error("Cannot load OpenGL functions from the current context");
    }
    GLint major = 0, minor = 0;
    functions_.gl.GetIntegerv(GL_MAJOR_VERSION, &major);
    functions_.gl.GetIntegerv(GL_MINOR_VERSION, &minor);
    if (major < 3 || (major == 3 && minor < 3)) {
        throw std::runtime_error("Graphics requires OpenGL 3.3 or newer");
    }
    auto previous = gl3wProcs;
    gl3wProcs = &functions_;
    try {
        resources_.emplace();
    } catch (...) {
        gl3wProcs = previous;
        throw;
    }
}

Renderer::~Renderer() {
    auto previous = gl3wProcs;
    gl3wProcs = &functions_;
    resources_.reset();
    gl3wProcs = previous == &functions_ ? nullptr : previous;
}

void Renderer::render(const RenderContext&) {
}

void Renderer::begin(Target& target, const Math::Matrix4f& projection, const Math::Matrix4f& view) {
    target_ = &target;
    projection_ = projection;
    view_ = view;
    depth_test_ = true;
    bind();
}

void Renderer::bind() {
    if (target_ == nullptr) {
        throw std::logic_error("Call Renderer::begin before drawing or clearing");
    }
    gl3wProcs = &functions_;
    glBindFramebuffer(GL_FRAMEBUFFER, target_->framebuffer());
    glViewport(0, 0, target_->width(), target_->height());
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_FRAMEBUFFER_SRGB);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glDepthRange(0.0, 1.0);
    if (depth_test_) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::clear(const Math::Colorf& color) {
    bind();
    glClearColor(color.red() * color.alpha(), color.green() * color.alpha(), color.blue() * color.alpha(), color.alpha());
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(const Mesh& mesh, const Material& material, const Math::Affine3f& transform) {
    if (material.texture != nullptr && static_cast<const Target*>(material.texture) == target_) {
        throw std::invalid_argument("Cannot sample a Texture while rendering to it");
    }
    bind();
    if (mesh.empty()) return;
    resources_->program.use();
    resources_->program.uniformMatrix4fv("uProjection", projection_.data());
    resources_->program.uniformMatrix4fv("uView", view_.data());
    resources_->program.uniformMatrix4fv("uModel", transform.data());
    resources_->program.uniform4f("uColor", material.color.red(), material.color.green(), material.color.blue(), material.color.alpha());
    resources_->program.uniform("uTextured", material.texture != nullptr ? 1 : 0);
    resources_->program.uniform("uTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindSampler(0, 0);
    if (material.texture != nullptr) {
        material.texture->texture_.bind(0);
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    mesh.vertexArray().bind();
    glDrawElements(GL_TRIANGLES, mesh.count(), GL_UNSIGNED_INT, nullptr);
    gl::VertexArray::unbind();
    gl::Program::unuse();
}

void Renderer::draw(const Texture& texture, const Math::Affine3f& transform, const Math::Colorf& color) {
    draw(resources_->quad, Material{color, &texture}, transform);
}

VOLCANO_GRAPHICS_END
