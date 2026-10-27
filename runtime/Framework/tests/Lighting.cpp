#define GLFW_INCLUDE_NONE

#include <array>
#include <cmath>
#include <limits>
#include <memory>
#include <span>
#include <stdexcept>
#include <string_view>
#include <vector>

#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

#include <Volcano/Graphics/FrameBuffer.h>
#include <Volcano/Graphics/Mesh.h>
#include <Volcano/Graphics/Pass.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Graphics/Texture.h>
#include <Volcano/Framework/Window.h>
#include <Volcano/ScopeGuard.h>

namespace {

using namespace Volcano::Graphics;

void buildView(View& view, std::span<const DrawItem> items = {}, std::span<const Light> lights = {}) {
    view.begin();
    for (const auto& item : items) {
        view.addItem(item.mesh.get(), item.material, item.transform, item.transparent);
    }
    for (const auto& light : lights) {
        view.addLight(light);
    }
    view.end();
}

PFNGLGETUNIFORMLOCATIONPROC real_uniform_location = nullptr;
int uniform_location_queries = 0;

GLint APIENTRY countUniformLocationQueries(GLuint program, const GLchar* name) {
    ++uniform_location_queries;
    return real_uniform_location(program, name);
}

PFNGLUSEPROGRAMPROC real_use_program = nullptr;
int program_bindings = 0;
int program_unbindings = 0;

void APIENTRY countProgramBindings(GLuint program) {
    if (program == 0) {
        ++program_unbindings;
    } else {
        ++program_bindings;
    }

    real_use_program(program);
}

PFNGLGETFRAGDATALOCATIONPROC real_fragment_location = nullptr;
int fragment_location_queries = 0;

GLint APIENTRY rejectSecondFragmentOutput(GLuint program, const GLchar* name) {
    ++fragment_location_queries;
    if (fragment_location_queries == 2) {
        return -1;
    }

    return real_fragment_location(program, name);
}

class TintPass final : public Pass {
public:
    using Pass::ObjectUniforms;

public:
    TintPass(std::string_view vertexSource, std::string_view fragmentSource,
        std::string_view vertexDefinitions = {}, std::string_view fragmentDefinitions = {}) {
        attachShader(GL_VERTEX_SHADER, "330 core", vertexSource, Mesh::shaderDefinitions() + std::string(vertexDefinitions));
        attachShader(GL_FRAGMENT_SHADER, "330 core", fragmentSource, "#define COLOR_OUTPUT 0\n" + std::string(fragmentDefinitions));
        program_.link();
        tint_ = program_.uniformLocation("uTint", GL_FLOAT_VEC3);
    }

    void render(const Mesh& mesh) {
        program_.use();
        program_.uniform3f(tint_, 1.0f, 0.5f, 0.0f);
        mesh.draw();
        gl::Program::unuse();
    }

private:
    GLint tint_;
};

class RendererLighting : public ::testing::Test {
protected:
    void SetUp() override {
        ASSERT_TRUE(glfwInit());
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        window_ = std::make_unique<Volcano::Framework::Window>(32, 32, "Lighting check");
        renderer_ = std::make_unique<Renderer>(glfwGetProcAddress);
        constexpr Mesh::Vertex vertices[] = {
            {{-1, -1, 0}}, {{1, -1, 0}}, {{1, 1, 0}}, {{-1, 1, 0}}
        };
        constexpr Mesh::index_type indices[] = {0, 1, 2, 0, 2, 3};
        mesh_ = std::make_unique<Mesh>(vertices, indices);
        target_ = std::make_unique<FrameBuffer>(16, 16);
    }

    void TearDown() override {
        target_.reset();
        mesh_.reset();
        renderer_.reset();
        window_.reset();
        glfwTerminate();
    }

    std::array<unsigned char, 4> pixel(int x = 8) {
        std::array<unsigned char, 4> result{};
        glReadPixels(x, 8, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, result.data());
        EXPECT_EQ(glGetError(), GL_NO_ERROR);
        return result;
    }

    void renderEmpty(const Volcano::Colorf& color = Volcano::Colorf::Black) {
        View view;
        view.setClearColor(color);
        buildView(view);
        renderer_->render(view, *target_);
    }

    void expectColor(float red, float green, float blue, int x = 8) {
        const auto result = pixel(x);
        EXPECT_NEAR(result[0], red * 255.0f, 2.0f);
        EXPECT_NEAR(result[1], green * 255.0f, 2.0f);
        EXPECT_NEAR(result[2], blue * 255.0f, 2.0f);
    }

protected:
    std::unique_ptr<Volcano::Framework::Window> window_;
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<Mesh> mesh_;
    std::unique_ptr<FrameBuffer> target_;
};

TEST_F(RendererLighting, SupportsGeometryStageAndPassSelectedGlslVersion) {
    class GeometryTestPass final : public Pass {
    public:
        GeometryTestPass() {
            attachShader(GL_VERTEX_SHADER, "150", R"GLSL(
void main() {
    vec2 point = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);
    gl_Position = vec4(point * 2.0 - 1.0, 0.0, 1.0);
}
)GLSL");
            attachShader(GL_GEOMETRY_SHADER, "150", R"GLSL(
#if defined(POSITION_ATTRIBUTE) || defined(COLOR_OUTPUT) || defined(ALBEDO_ATTACHMENT)
#error Vertex and fragment definitions must not enter the geometry stage
#endif
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
void main() {
    for (int i = 0; i < 3; ++i) {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
)GLSL");
            attachShader(GL_FRAGMENT_SHADER, "150", R"GLSL(
out vec4 color;
void main() {
    color = vec4(0.0, 1.0, 0.0, 1.0);
}
)GLSL");
            program_.link();
        }

        void render() {
            program_.use();
            gl::VertexArray vertices;
            vertices.bind();
            glDrawArrays(GL_TRIANGLES, 0, 3);
            gl::VertexArray::unbind();
            gl::Program::unuse();
        }
    };

    GeometryTestPass pass;
    renderEmpty();
    pass.render();
    expectColor(0, 1, 0);
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
}

TEST_F(RendererLighting, PassSelectsDefinitionsAndBindsItsOwnParameters) {
    TintPass pass(R"GLSL(
#if defined(COLOR_OUTPUT) || defined(TINT_SCALE) || defined(ALBEDO_ATTACHMENT)
#error Fragment definitions must not enter the vertex stage
#endif
layout(location = POSITION_ATTRIBUTE) in vec3 aPosition;
void main() {
    gl_Position = vec4(aPosition * VERTEX_SCALE, 1.0);
}
)GLSL", R"GLSL(
#if defined(POSITION_ATTRIBUTE) || defined(VERTEX_SCALE) || defined(ALBEDO_ATTACHMENT)
#error Vertex definitions must not enter the fragment stage
#endif
layout(location = COLOR_OUTPUT) out vec4 color;
void main() {
    color = vec4(uTint * TINT_SCALE, 1.0);
}
)GLSL", "#define VERTEX_SCALE 0.5", "#define TINT_SCALE 0.5\nuniform vec3 uTint;");

    renderEmpty();
    pass.render(*mesh_);
    expectColor(0.5f, 0.25f, 0.0f);
    expectColor(0, 0, 0, 1);
}

TEST_F(RendererLighting, PassRejectsShaderCompilationAndLinkFailures) {
    constexpr auto vertex = "void main() { gl_Position = vec4(0.0); }";
    constexpr auto fragment = "out vec4 color; void main() { color = vec4(1.0); }";
    EXPECT_THROW((TintPass("invalid shader", fragment)), std::runtime_error);
    EXPECT_THROW((TintPass(vertex, "invalid shader")), std::runtime_error);
    EXPECT_THROW((TintPass(R"GLSL(
out vec3 value;
void main() {
    value = vec3(1.0);
    gl_Position = vec4(0.0);
}
)GLSL", R"GLSL(
in vec4 value;
uniform vec3 uTint;
out vec4 color;
void main() {
    color = value * vec4(uTint, 1.0);
}
)GLSL")), std::runtime_error);
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
}

TEST_F(RendererLighting, PassesExchangeGeometryWithoutHoldingOtherPasses) {
    View view;
    std::vector<DrawItem> items;
    view.setAmbientLight(Eigen::Vector3f::Constant(0.5f));
    items.push_back({*mesh_, {Volcano::Colorf(1, 0, 0, 0.5f), nullptr, true},
        Eigen::Affine3f::Identity(), true});
    {
        TransparentPass pass;
        renderEmpty();
        buildView(view, items);
        pass.render(view);
        expectColor(0.25f, 0, 0);
    }
    GBuffer geometry;
    geometry.resize(target_->size());
    items[0].transparent = false;
    items[0].material.color = Volcano::Colorf(0, 2, 0);
    view.setAmbientLight(Eigen::Vector3f::Constant(0.25f));
    buildView(view, items);
    {
        GeometryPass pass;
        geometry.framebuffer().bind();
        glDepthMask(GL_TRUE);
        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        pass.render(view);
        std::array<float, 4> value{};
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glReadPixels(8, 8, 1, 1, GL_RGBA, GL_FLOAT, value.data());
        EXPECT_FLOAT_EQ(value[1], 2.0f); // Geometry stores unclamped albedo without lighting.
        glReadBuffer(GL_COLOR_ATTACHMENT1);
        glReadPixels(8, 8, 1, 1, GL_RGBA, GL_FLOAT, value.data());
        EXPECT_FLOAT_EQ(value[2], 1.0f);
        EXPECT_FLOAT_EQ(value[3], 1.0f);
        glReadBuffer(GL_COLOR_ATTACHMENT2);
        glReadPixels(8, 8, 1, 1, GL_RGBA, GL_FLOAT, value.data());
        EXPECT_NEAR(value[0], 0.0625f, 0.00001f);
        EXPECT_NEAR(value[1], 0.0625f, 0.00001f);
    }
    {
        LightingPass pass;
        renderEmpty();
        pass.render(view, geometry);
        expectColor(0, 0.5f, 0);
        float depth = 0;
        glReadPixels(8, 8, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
        EXPECT_NEAR(depth, 0.5f, 0.00001f);
    }
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
}

TEST_F(RendererLighting, GBufferResizePreservesStorageOnFailureAndReusesMatchingSize) {
    GBuffer geometry;
    EXPECT_TRUE(geometry.size().empty());
    geometry.resize({8, 8});
    const auto texture = geometry.albedo().id();
    const auto framebuffer = geometry.framebuffer().id();
    geometry.resize({8, 8});
    EXPECT_EQ(geometry.albedo().id(), texture);
    EXPECT_EQ(geometry.framebuffer().id(), framebuffer);
    EXPECT_THROW(geometry.resize({0, 8}), std::invalid_argument);
    EXPECT_THROW(geometry.resize({8, -1}), std::invalid_argument);
    GLint maximum = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maximum);
    EXPECT_THROW(geometry.resize({maximum + 1, 8}), std::invalid_argument);
    EXPECT_EQ(geometry.size(), (Volcano::Size{8, 8}));
    EXPECT_EQ(geometry.albedo().id(), texture);
    EXPECT_EQ(geometry.framebuffer().id(), framebuffer);
    geometry.resize({16, 12});
    EXPECT_EQ(geometry.size(), (Volcano::Size{16, 12}));
    EXPECT_TRUE(geometry.framebuffer().complete());
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
}

TEST_F(RendererLighting, ChangesLightCapacityInBothPasses) {
    auto* dispatch = gl3wProcs;
    auto restore = Volcano::scopeGuard([dispatch]() noexcept {
        gl3wProcs = dispatch;
    });
    Renderer configured(glfwGetProcAddress);
    EXPECT_EQ(configured.maxLights(), 16);
    gl3wProcs = dispatch;
    configured.setMaxLights(2);
    EXPECT_EQ(configured.maxLights(), 2);
    EXPECT_EQ(gl3wProcs, dispatch);

    View view;
    std::vector<DrawItem> items;
    std::vector<Light> lights;
    items.push_back({*mesh_, {Volcano::Colorf::Red, nullptr, true}});
    items.push_back({*mesh_, {Volcano::Colorf(0, 0, 1, 0.5f), nullptr, true}, Eigen::Affine3f::Identity(), true});
    items[0].transform.translation().z() = 0.5f;
    lights.resize(2);
    for (auto& light : lights) {
        light.direction = {0, 0, -1};
        light.strength = 0.25f;
    }

    buildView(view, items, lights);
    configured.render(view, *target_);
    expectColor(0.25f, 0, 0.25f);
    lights.push_back(lights.front());
    EXPECT_THROW({
        buildView(view, items, lights);
        configured.render(view, *target_);
    }, std::invalid_argument);
    expectColor(0.25f, 0, 0.25f);

    EXPECT_THROW(configured.setMaxLights(0), std::invalid_argument);
    EXPECT_THROW(configured.setMaxLights(std::numeric_limits<std::size_t>::max()), std::invalid_argument);
    EXPECT_EQ(configured.maxLights(), 2);
    lights.pop_back();
    buildView(view, items, lights);
    configured.render(view, *target_);
    expectColor(0.25f, 0, 0.25f);

    lights.push_back(lights.front());
    configured.setMaxLights(3);
    EXPECT_EQ(configured.maxLights(), 3);
    buildView(view, items, lights);
    configured.render(view, *target_);
    expectColor(0.375f, 0, 0.375f);
    configured.setMaxLights(3);
    buildView(view, items, lights);
    configured.render(view, *target_);
    expectColor(0.375f, 0, 0.375f);
    configured.setMaxLights(2);
    EXPECT_THROW({
        buildView(view, items, lights);
        configured.render(view, *target_);
    }, std::invalid_argument);

    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(0.375f, 0, 0.375f);
}

TEST_F(RendererLighting, KeepsBothPassesWhenChangingCapacityFails) {
    renderer_->setMaxLights(2);
    real_fragment_location = gl3wProcs->gl.GetFragDataLocation;
    fragment_location_queries = 0;
    {
        auto restore = Volcano::scopeGuard([]() noexcept {
            gl3wProcs->gl.GetFragDataLocation = real_fragment_location;
        });
        gl3wProcs->gl.GetFragDataLocation = rejectSecondFragmentOutput;
        EXPECT_THROW(renderer_->setMaxLights(3), std::runtime_error);
        EXPECT_EQ(fragment_location_queries, 2);
        EXPECT_EQ(renderer_->maxLights(), 2);
    }

    View view;
    std::vector<DrawItem> items;
    std::vector<Light> lights;
    items.push_back({*mesh_, {Volcano::Colorf::Red, nullptr, true}});
    items.push_back({*mesh_, {Volcano::Colorf(0, 0, 1, 0.5f), nullptr, true}, Eigen::Affine3f::Identity(), true});
    items[0].transform.translation().z() = 0.5f;
    lights.resize(2);
    for (auto& light : lights) {
        light.direction = {0, 0, -1};
        light.strength = 0.25f;
    }

    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(0.25f, 0, 0.25f);
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
}

TEST_F(RendererLighting, AcceptsAConfiguredCapacityAboveTheDefault) {
    auto* dispatch = gl3wProcs;
    auto restore = Volcano::scopeGuard([dispatch]() noexcept {
        gl3wProcs = dispatch;
    });
    Renderer configured(glfwGetProcAddress);
    configured.setMaxLights(24);
    EXPECT_EQ(configured.maxLights(), 24);
    View view;
    std::vector<DrawItem> items;
    std::vector<Light> lights;
    items.push_back({*mesh_, {Volcano::Colorf::White, nullptr, true}});
    lights.resize(24);
    for (auto& light : lights) {
        light.direction = {0, 0, -1};
        light.strength = 0.5f / 24;
    }

    buildView(view, items, lights);
    configured.render(view, *target_);
    expectColor(0.5f, 0.5f, 0.5f);
}

TEST_F(RendererLighting, BindsEachPassProgramOnceForMultipleItems) {
    real_use_program = gl3wProcs->gl.UseProgram;
    program_bindings = 0;
    program_unbindings = 0;
    auto restore = Volcano::scopeGuard([]() noexcept {
        gl3wProcs->gl.UseProgram = real_use_program;
    });
    gl3wProcs->gl.UseProgram = countProgramBindings;

    View view;
    std::vector<DrawItem> items;
    for (int i = 0; i < 4; ++i) {
        items.push_back({*mesh_, {Volcano::Colorf::Red}});
        items.push_back({*mesh_, {Volcano::Colorf(0, 0, 1, 0.5f)}, Eigen::Affine3f::Identity(), true});
        items.back().transform.translation().z() = -0.5f;
    }

    buildView(view, items);
    renderer_->render(view, *target_);
    EXPECT_EQ(program_bindings, 3);
    EXPECT_EQ(program_unbindings, 3);
    expectColor(0.0625f, 0, 0.9375f);
    GLint current = -1;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    EXPECT_EQ(current, 0);
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
}

TEST_F(RendererLighting, RequiresCompletedViewAndReusesItAcrossTargets) {
    renderEmpty(Volcano::Colorf::Blue);
    View view;
    std::vector<DrawItem> items;
    EXPECT_THROW(renderer_->render(view, *target_), std::logic_error);
    GeometryPass geometry;
    LightingPass lighting;
    TransparentPass transparent;
    EXPECT_THROW(geometry.render(view), std::logic_error);
    EXPECT_THROW(lighting.render(view, renderer_->geometryBuffer()), std::logic_error);
    EXPECT_THROW(transparent.render(view), std::logic_error);
    expectColor(0, 0, 1);

    items.push_back({*mesh_, {Volcano::Colorf::Red}});
    buildView(view, items);
    const View& input = view;
    const auto* prepared = input.opaqueItems().data();
    renderer_->render(input, *target_);
    expectColor(1, 0, 0);
    FrameBuffer other(16, 16);
    renderer_->render(input, other);
    expectColor(1, 0, 0);
    EXPECT_TRUE(input.ready());
    EXPECT_EQ(input.opaqueItems().data(), prepared);

    items[0].material.color = Volcano::Colorf::Green;
    view.begin();
    EXPECT_THROW(renderer_->render(input, other), std::logic_error);
    expectColor(1, 0, 0);
    buildView(view, items);
    renderer_->render(input, other);
    expectColor(0, 1, 0);
}

TEST_F(RendererLighting, RejectsInvalidTransformsAtEnd) {
    View view;
    std::vector<DrawItem> items;
    items.push_back({*mesh_, {Volcano::Colorf::White, nullptr, true}});
    items.front().transform.linear()(0, 0) = 0;
    EXPECT_THROW(buildView(view, items), std::invalid_argument);
    items.front().transparent = true;
    EXPECT_THROW(buildView(view, items), std::invalid_argument);
    GLint current = -1;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    EXPECT_EQ(current, 0);
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
}

TEST_F(RendererLighting, ReusesUniformLocationsAcrossFramesAndMaterialChanges) {
    real_uniform_location = gl3wProcs->gl.GetUniformLocation;
    uniform_location_queries = 0;
    auto restore = Volcano::scopeGuard([]() noexcept {
        gl3wProcs->gl.GetUniformLocation = real_uniform_location;
    });
    gl3wProcs->gl.GetUniformLocation = countUniformLocationQueries;
    renderer_->setMaxLights(renderer_->maxLights());

    View view;
    std::vector<DrawItem> items;
    items.push_back({*mesh_, {Volcano::Colorf::Red}});
    buildView(view, items);
    renderer_->render(view, *target_);
    expectColor(1, 0, 0);

    items[0].material.color = Volcano::Colorf::Blue;
    items[0].transparent = true;
    buildView(view, items);
    renderer_->render(view, *target_);
    expectColor(0, 0, 1);
    EXPECT_EQ(uniform_location_queries, 0);
}

TEST_F(RendererLighting, ValidatesLinkedShaderUniformsAttributesAndOptionalInputs) {
    gl::Program program;
    program.attachSource(GL_VERTEX_SHADER, R"GLSL(
#version 330 core
layout(location = 0) in vec3 aPosition;
uniform mat4 uProjection;
uniform float uSamples[2];
uniform float uUnused;
void main() {
    gl_Position = uProjection * vec4(aPosition * uSamples[gl_VertexID & 1], 1.0);
}
)GLSL");
    program.attachSource(GL_FRAGMENT_SHADER, R"GLSL(
#version 330 core
layout(location = 2) out vec4 color;
void main() {
    color = vec4(1.0);
}
)GLSL");
    program.link();

    EXPECT_GE(program.uniformLocation("uProjection", GL_FLOAT_MAT4), 0);
    EXPECT_GE(program.uniformLocation("uSamples[0]", GL_FLOAT, 2), 0);
    EXPECT_THROW(program.uniformLocation("uSamples[0]", GL_FLOAT, 3), std::runtime_error);
    EXPECT_THROW(program.uniformLocation("uProjection", GL_FLOAT), std::runtime_error);
    EXPECT_THROW(program.uniformLocation("uProjection", GL_FLOAT_MAT4, 0), std::invalid_argument);
    EXPECT_THROW(program.uniformLocation("uMissing", GL_FLOAT), std::runtime_error);
    EXPECT_THROW(program.uniformLocation("uUnused", GL_FLOAT), std::runtime_error);
    EXPECT_EQ(program.uniformLocation("uUnused", GL_FLOAT, 1, false), -1);
    EXPECT_EQ(program.attributeLocation("aPosition", GL_FLOAT_VEC3, Mesh::PositionAttribute), 0);
    EXPECT_THROW(program.attributeLocation("aPosition", GL_FLOAT_VEC2, Mesh::PositionAttribute), std::runtime_error);
    EXPECT_THROW(program.attributeLocation("aPosition", GL_FLOAT_VEC3, Mesh::NormalAttribute), std::runtime_error);
    EXPECT_THROW(program.attributeLocation("aMissing", GL_FLOAT_VEC3, 0), std::runtime_error);

    EXPECT_EQ(program.fragmentLocation("color"), 2);
    EXPECT_EQ(program.fragmentLocation("color", 2), 2);
    EXPECT_THROW(program.fragmentLocation("color", 0), std::runtime_error);
    EXPECT_EQ(program.fragmentLocation("missing"), -1);
    EXPECT_THROW(program.fragmentLocation("missing", 0), std::runtime_error);

    TintPass::ObjectUniforms uniforms;
    EXPECT_THROW(uniforms.resolve(program), std::runtime_error);
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
}


TEST_F(RendererLighting, LightingPassRejectsUnavailableResourcesFeedbackAndMismatchedViewport) {
    LightingPass pass;
    View view;
    buildView(view);
    renderEmpty(Volcano::Colorf::Blue);
    GBuffer geometry;
    EXPECT_THROW(pass.render(view, geometry), std::invalid_argument);
    geometry.resize(target_->size());
    EXPECT_THROW(pass.render(view, geometry), std::invalid_argument);
    renderEmpty(Volcano::Colorf::Blue);
    glViewport(1, 0, target_->width(), target_->height());
    EXPECT_THROW(pass.render(view, geometry), std::invalid_argument);
    glViewport(0, 0, target_->width(), target_->height());
    expectColor(0, 0, 1);
}

TEST_F(RendererLighting, KeepsGBufferStorageAndCanRenderAgainAfterResizeFailure) {
    View view;
    std::vector<DrawItem> items;
    items.push_back({*mesh_});
    buildView(view, items);
    renderer_->render(view, *target_);
    EXPECT_EQ(renderer_->geometryBuffer().size(), target_->size());
    const auto original = renderer_->geometryBuffer().albedo().id();
    FrameBuffer other(24, 32);
    {
        auto& check = gl3wProcs->gl.CheckFramebufferStatus;
        const auto saved = check;
        auto restore = Volcano::scopeGuard([&check, saved]() noexcept {
            check = saved;
        });
        check = [](GLenum) -> GLenum {
            return GL_FRAMEBUFFER_UNSUPPORTED;
        };
        EXPECT_THROW({
            buildView(view, items);
            renderer_->render(view, other);
        }, std::runtime_error);
    }
    EXPECT_EQ(renderer_->geometryBuffer().size(), target_->size());
    EXPECT_EQ(renderer_->geometryBuffer().albedo().id(), original);
    buildView(view, items);
    renderer_->render(view, other);
    EXPECT_EQ(renderer_->geometryBuffer().size(), other.size());
    expectColor(1, 1, 1);
}

TEST_F(RendererLighting, RejectsNonfiniteTransparentDepthBeforeClearingAndCanRecover) {
    renderEmpty(Volcano::Colorf::Blue);
    View view;
    std::vector<DrawItem> items;
    Eigen::Matrix4f camera = Eigen::Matrix4f::Identity();
    items.push_back({*mesh_, {Volcano::Colorf::Red}});
    items.push_back({*mesh_, {Volcano::Colorf::Blue}, Eigen::Affine3f::Identity(), true});
    camera(2, 2) = std::numeric_limits<float>::max();
    items[1].transform.translation().z() = std::numeric_limits<float>::max();
    EXPECT_THROW({
        view.setViewMatrix(camera);
        buildView(view, items);
        renderer_->render(view, *target_);
    }, std::invalid_argument);
    expectColor(0, 0, 1);

    camera.setIdentity();
    items.pop_back();
    items[0].material.color = Volcano::Colorf::Green;
    view.setViewMatrix(camera);
    buildView(view, items);
    renderer_->render(view, *target_);
    expectColor(0, 1, 0);
}

TEST_F(RendererLighting, ClearsBackgroundAndDepthWhenGeometryDisappears) {
    View view;
    std::vector<DrawItem> items;
    view.setClearColor(Volcano::Colorf(0, 0, 1, 0.5f));
    items.push_back({*mesh_, {Volcano::Colorf::Green}});
    items[0].transform.linear().diagonal().setConstant(0.5f);
    buildView(view, items);
    renderer_->render(view, *target_);
    expectColor(0, 1, 0);
    expectColor(0, 0, 0.5f, 0);
    items.clear();
    buildView(view, items);
    renderer_->render(view, *target_);
    expectColor(0, 0, 0.5f);
    EXPECT_NEAR(pixel()[3], 127.5f, 1.0f);
    float depth = 0;
    glReadPixels(8, 8, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    EXPECT_FLOAT_EQ(depth, 1.0f);
}

TEST_F(RendererLighting, ReusesViewAcrossTargetsAndChecksFeedbackAgainstOutput) {
    View view;
    std::vector<DrawItem> items;
    items.push_back({*mesh_, {Volcano::Colorf::Red}});
    buildView(view, items);
    renderer_->render(view, *target_);
    expectColor(1, 0, 0);
    GLint saved_target = 0;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &saved_target);

    FrameBuffer other(32, 24);
    items[0].material.color = Volcano::Colorf::Blue;
    buildView(view, items);
    renderer_->render(view, other);
    expectColor(0, 0, 1);
    GLint viewport[4]{};
    glGetIntegerv(GL_VIEWPORT, viewport);
    EXPECT_EQ(viewport[2], other.width());
    EXPECT_EQ(viewport[3], other.height());
    glBindFramebuffer(GL_READ_FRAMEBUFFER, saved_target);
    expectColor(1, 0, 0);

    items[0].material = {Volcano::Colorf::White, &target_->texture()};
    EXPECT_THROW({
        buildView(view, items);
        renderer_->render(view, *target_);
    }, std::invalid_argument);
    expectColor(1, 0, 0);
    buildView(view, items);
    renderer_->render(view, other);
    expectColor(1, 0, 0);
    buildView(view, items);
    renderer_->render(view, *window_);
    expectColor(1, 0, 0);
    GLint framebuffer = -1;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &framebuffer);
    EXPECT_EQ(framebuffer, 0);
    EXPECT_EQ(view.items().size(), 1);
}

TEST_F(RendererLighting, SamplesTextureWithoutChangingOutputInBothStages) {
    constexpr std::array<unsigned char, 4> rgba{255, 0, 0, 128};
    Texture texture(1, 1, rgba);
    View view;
    std::vector<DrawItem> items;
    view.setClearColor(Volcano::Colorf::Blue);
    items.push_back({*mesh_, {Volcano::Colorf::White, &texture}});
    buildView(view, items);
    renderer_->render(view, *target_);
    expectColor(128.0f / 255.0f, 0, 0);
    EXPECT_EQ(pixel()[3], 128);

    items[0].transparent = true;
    buildView(view, items);
    renderer_->render(view, *target_);
    expectColor(128.0f / 255.0f, 0, 127.0f / 255.0f);
    EXPECT_EQ(pixel()[3], 255);
}

TEST_F(RendererLighting, OpaqueAndTransparentProgramsUpdateTheirOwnLighting) {
    View view;
    std::vector<DrawItem> items;
    std::vector<Light> lights;
    items.push_back({*mesh_, {Volcano::Colorf::Red, nullptr, true}});
    items.push_back({*mesh_, {Volcano::Colorf(0, 0, 1, 0.5f), nullptr, true}, Eigen::Affine3f::Identity(), true});
    // S * R * T scales translation too; keep the model origins at X = +/-0.5.
    items[0].transform.translation() = Eigen::Vector3f(-1.25f, 0, 0);
    items[0].transform.linear().diagonal().setConstant(0.4f);
    items[1].transform.translation() = Eigen::Vector3f(1.25f, 0, 0);
    items[1].transform.linear().diagonal().setConstant(0.4f);
    view.setAmbientLight(Eigen::Vector3f::Constant(0.5f));
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(0.5f, 0, 0, 4);
    expectColor(0, 0, 0.25f, 12);

    view.setAmbientLight(Eigen::Vector3f::Zero());
    items[0].material.color = Volcano::Colorf::White;
    items[1].material.color = Volcano::Colorf(1, 1, 1, 0.5f);
    Light light;
    light.direction = {0, 0, -1};
    light.color = {0.2f, 0.4f, 0.6f};
    lights.push_back(light);
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(0.2f, 0.4f, 0.6f, 4);
    expectColor(0.1f, 0.2f, 0.3f, 12);

    lights.clear();
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(0, 0, 0, 4);
    expectColor(0, 0, 0, 12);
}

TEST_F(RendererLighting, AmbientDirectionalAndUnlitMaterials) {
    View view;
    std::vector<DrawItem> items;
    std::vector<Light> lights;
    items.push_back({*mesh_});
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(1, 1, 1);
    items[0].material.lit = true;
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(0, 0, 0);
    view.setAmbientLight({0.1f, 0.2f, 0.3f});
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(0.1f, 0.2f, 0.3f);
    Light light;
    light.direction = {0, 0, -10};
    light.color = {0.2f, 0.3f, 0.4f};
    lights.push_back(light);
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(0.3f, 0.5f, 0.7f);
    lights.push_back(light);
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(0.5f, 0.8f, 1.0f);
    lights.resize(1);
    lights[0].direction.z() = 1;
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(0.1f, 0.2f, 0.3f);
    lights.clear();
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(0.1f, 0.2f, 0.3f);
    items[0].material.lit = false;
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(1, 1, 1);
}

TEST_F(RendererLighting, PointRangeAndSpotConeAttenuation) {
    View view;
    std::vector<DrawItem> items;
    std::vector<Light> lights;
    items.push_back({*mesh_});
    items[0].material.lit = true;
    Light light;
    light.type = Light::Type::Point;
    // The sampled pixel maps to (1/16, 1/16, 0) in world space.
    light.position = {0.0625f, 0.0625f, 1.0f};
    light.range = 2;
    lights.push_back(light);
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(0.25f, 0.25f, 0.25f);
    lights[0].range = 1;
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(0, 0, 0);
    lights[0].range = 2;
    lights[0].type = Light::Type::Spot;
    lights[0].direction = {0, 0, -1};
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(0.25f, 0.25f, 0.25f);
    const float angle = 22.5f * std::acos(-1.0f) / 180.0f;
    lights[0].direction = {std::sin(angle), 0, -std::cos(angle)};
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    const float inner = std::cos(15.0f * std::acos(-1.0f) / 180.0f);
    const float outer = std::cos(30.0f * std::acos(-1.0f) / 180.0f);
    const float t = (std::cos(angle) - outer) / (inner - outer);
    const float expected = 0.25f * t * t * (3 - 2 * t);
    expectColor(expected, expected, expected);
    lights[0].direction = {1, 0, 0};
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(0, 0, 0);
}

TEST_F(RendererLighting, UsesInverseTransposeNormals) {
    constexpr Mesh::Vertex vertices[] = {
        {{-1, -1, 0}, {1, 0, 1}}, {{1, -1, 0}, {1, 0, 1}},
        {{1, 1, 0}, {1, 0, 1}}, {{-1, 1, 0}, {1, 0, 1}}
    };
    constexpr Mesh::index_type indices[] = {0, 1, 2, 0, 2, 3};
    Mesh mesh(vertices, indices);
    View view;
    std::vector<DrawItem> items;
    std::vector<Light> lights;
    items.push_back({mesh});
    items[0].material.lit = true;
    items[0].transform.linear().diagonal() = Eigen::Vector3f(2, 1, 1);
    Light light;
    light.direction = {0, 0, -1};
    lights.push_back(light);
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    const float expected = 1.0f / std::sqrt(1.25f);
    expectColor(expected, expected, expected);
}

TEST_F(RendererLighting, SortsTransparentItemsAndRetainsOpaqueDepth) {
    View view;
    std::vector<DrawItem> items;
    std::vector<Light> lights;
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
    items.push_back({*mesh_, {Volcano::Colorf(1, 0, 0, 0.5f)}, Eigen::Affine3f::Identity(), true});
    items[0].transform.translation() = Eigen::Vector3f(0, 0, -0.5f);
    items.push_back({*mesh_, {Volcano::Colorf(0, 0, 1, 0.5f)}, Eigen::Affine3f::Identity(), true});
    items[1].transform.translation() = Eigen::Vector3f(0, 0, 0.5f);
    // Camera looks along -Z; reverse Z in the orthographic projection for GL depth.
    projection(2, 2) = -1;
    items[0].material.lit = true;
    Light light;
    light.direction = {0, 0, -1};
    lights.push_back(light);
    view.setProjection(projection);
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(0.25f, 0, 0.5f);
    EXPECT_FLOAT_EQ(view.items()[0].transform.translation().z(), -0.5f);
    GLboolean writes = GL_FALSE;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &writes);
    EXPECT_EQ(writes, GL_TRUE);
    float depth = 0;
    glReadPixels(8, 8, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    EXPECT_FLOAT_EQ(depth, 1.0f);
    items.push_back({*mesh_, {Volcano::Colorf::Green}});
    view.setProjection(projection);
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(0, 0.5f, 0.5f);
    glReadPixels(8, 8, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    EXPECT_NEAR(depth, 0.5f, 0.0001f);
}

TEST_F(RendererLighting, SortsTransparentItemsUsingGeneratedAffineOrigin) {
    View view;
    std::vector<DrawItem> items;
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
    items.push_back({*mesh_, {Volcano::Colorf(1, 0, 0, 0.5f)}, Eigen::Affine3f::Identity(), true});
    items.push_back({*mesh_, {Volcano::Colorf(0, 0, 1, 0.5f)}, Eigen::Affine3f::Identity(), true});
    items[0].transform.translation().z() = -0.5f;
    items[1].transform.translation().z() = 0.5f;
    for (auto& item : items) {
        item.transform.linear()(2, 2) = -1;
    }

    // Negative Z scale reverses the order of the stored translation components.
    projection(2, 2) = -1;
    view.setProjection(projection);
    buildView(view, items);
    renderer_->render(view, *target_);
    expectColor(0.5f, 0, 0.25f);
}

TEST_F(RendererLighting, RejectsInvalidInputBeforeClearingAndCanRecover) {
    renderEmpty(Volcano::Colorf::Blue);
    View view;
    std::vector<DrawItem> items;
    std::vector<Light> lights;
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
    lights.resize(renderer_->maxLights() + 1);
    EXPECT_THROW({
        view.setProjection(projection);
        buildView(view, items, lights);
        renderer_->render(view, *target_);
    }, std::invalid_argument);
    expectColor(0, 0, 1);
    lights.clear();
    items.push_back({*mesh_});
    items[0].material.lit = true;
    items[0].transform.linear().setZero();
    EXPECT_THROW({
        view.setProjection(projection);
        buildView(view, items, lights);
        renderer_->render(view, *target_);
    }, std::invalid_argument);
    expectColor(0, 0, 1);
    items[0].transform.setIdentity();

    items[0].transform.translation().x() = std::numeric_limits<float>::quiet_NaN();
    EXPECT_THROW({
        view.setProjection(projection);
        buildView(view, items, lights);
        renderer_->render(view, *target_);
    }, std::invalid_argument);
    expectColor(0, 0, 1);
    items[0].transform.setIdentity();

    items[0].transform.linear()(0, 0) = std::numeric_limits<float>::quiet_NaN();
    EXPECT_THROW({
        view.setProjection(projection);
        buildView(view, items, lights);
        renderer_->render(view, *target_);
    }, std::invalid_argument);
    expectColor(0, 0, 1);
    items[0].transform.setIdentity();

    items[0].material.texture = &target_->texture();
    EXPECT_THROW({
        view.setProjection(projection);
        buildView(view, items, lights);
        renderer_->render(view, *target_);
    }, std::invalid_argument);
    expectColor(0, 0, 1);
    items[0].material.texture = nullptr;
    projection(0, 0) = std::numeric_limits<float>::quiet_NaN();
    EXPECT_THROW({
        view.setProjection(projection);
        buildView(view, items, lights);
        renderer_->render(view, *target_);
    }, std::invalid_argument);
    expectColor(0, 0, 1);
    projection.setIdentity();
    view.setAmbientLight({0.2f, 0.3f, 0.4f});
    view.setProjection(projection);
    buildView(view, items, lights);
    renderer_->render(view, *target_);
    expectColor(0.2f, 0.3f, 0.4f);
}

}
