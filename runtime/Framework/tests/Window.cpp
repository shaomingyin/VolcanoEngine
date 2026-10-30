#define GLFW_INCLUDE_NONE
#include <array>
#include <memory>
#include <stdexcept>
#include <cstring>
#include <type_traits>

#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Graphics/Mesh.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Graphics/FrameBuffer.h>
#include <Volcano/Graphics/Texture.h>
#include <Volcano/Framework/Window.h>

TEST(FrameworkWindow, ViewOwnsSubmittedDrawingState) {
    ASSERT_TRUE(glfwInit());
    auto glfw_guard = Volcano::scopeGuard([] {
        glfwTerminate();
    });
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    Volcano::Framework::Window window(32, 32, "View ownership");
    Volcano::Graphics::Renderer renderer;
    Volcano::Graphics::Mesh first({}, {});
    Volcano::Graphics::Mesh second({}, {});
    Volcano::Graphics::View view;
    EXPECT_TRUE(view.items().empty());
    static_assert(std::is_same_v<decltype(Volcano::Graphics::DrawItem::transform), Eigen::Affine3f>);
    EXPECT_TRUE(view.projection().isIdentity());
    EXPECT_TRUE(view.viewMatrix().isIdentity());
    Volcano::Transform transform;
    transform.setTranslation({1, 2, 3});
    view.begin();
    view.add(first, {Volcano::Colorf::Red}, transform, true);
    view.add(second);
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
    projection(0, 0) = 2;
    view.setProjection(projection);
    view.end();
    transform.setTranslation(Eigen::Vector3f::Zero());
    EXPECT_TRUE(view.items()[0].transform.translation().isApprox(Eigen::Vector3f(1, 2, 3)));
    EXPECT_EQ(view.transparentItems()[0].index, 0);
    EXPECT_EQ(view.opaqueItems()[0].index, 1);

    auto copy = view;
    copy.begin();
    copy.add(first, {Volcano::Colorf::Green});
    projection(0, 0) = 4;
    copy.setProjection(projection);
    copy.end();
    EXPECT_EQ(view.items().size(), 2);
    EXPECT_EQ(view.items()[0].material.color, Volcano::Colorf::Red);
    EXPECT_EQ(view.projection()(0, 0), 2);
    ASSERT_EQ(copy.items().size(), 1);
    EXPECT_EQ(&copy.items()[0].mesh.get(), &first);
    EXPECT_EQ(copy.projection()(0, 0), 4);
    view.begin();
    view.end();
    EXPECT_TRUE(view.items().empty());
    EXPECT_EQ(copy.items().size(), 1);
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
}

TEST(FrameworkWindow, RendersToDefaultFramebufferAndFrameBuffer) {
    ASSERT_TRUE(glfwInit());
    auto glfw_guard = Volcano::scopeGuard([] {
        glfwTerminate();
    });
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    Volcano::Framework::Window window(64, 48, "FrameworkWindowCheck");
    EXPECT_EQ(glfwGetCurrentContext(), window.handle());
    int width, height;
    glfwGetFramebufferSize(window.handle(), &width, &height);
    EXPECT_EQ(window.width(), width);
    EXPECT_EQ(window.height(), height);
    ASSERT_GT(width, 0);
    ASSERT_GT(height, 0);

    Volcano::Graphics::Renderer renderer;
    Volcano::Graphics::FrameBuffer texture(8, 8);
    Volcano::Graphics::View view;
    view.setClearColor(Volcano::Colorf::Red);
    view.begin();
    view.end();
    renderer.render(view, texture);
    constexpr Volcano::Graphics::Mesh::Vertex vertices[] = {
        {{0, 0, 0}, {0, 0, 1}, {0, 0}}, {{1, 0, 0}, {0, 0, 1}, {1, 0}},
        {{1, 1, 0}, {0, 0, 1}, {1, 1}}, {{0, 1, 0}, {0, 0, 1}, {0, 1}}
    };
    constexpr Volcano::Graphics::Mesh::index_type indices[] = {0, 1, 2, 0, 2, 3};
    Volcano::Graphics::Mesh quad(vertices, indices);
    view.setClearColor(Volcano::Colorf::Black);
    view.begin();
    view.add(quad, {Volcano::Colorf::White, &texture.texture()});
    view.end();
    renderer.render(view, window);
    GLint framebuffer = -1;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &framebuffer);
    EXPECT_EQ(framebuffer, 0);
    GLint viewport[4]{};
    glGetIntegerv(GL_VIEWPORT, viewport);
    EXPECT_EQ(viewport[2], width);
    EXPECT_EQ(viewport[3], height);
    glReadBuffer(GL_BACK);
    std::array<unsigned char, 4> pixel{};
    glReadPixels(width * 3 / 4, height * 3 / 4, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel.data());
    EXPECT_EQ(pixel[0], 255);
    EXPECT_EQ(pixel[1], 0);
    EXPECT_EQ(pixel[2], 0);
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
    glfwSwapBuffers(window.handle());
    glfwSetWindowShouldClose(window.handle(), GLFW_TRUE);
    EXPECT_TRUE(glfwWindowShouldClose(window.handle()));
}

TEST(FrameworkWindow, RejectsInvalidArguments) {
    EXPECT_THROW((Volcano::Framework::Window(0, 48, "invalid")), std::invalid_argument);
    EXPECT_THROW((Volcano::Framework::Window(64, -1, "invalid")), std::invalid_argument);
    EXPECT_THROW((Volcano::Framework::Window(64, 48, nullptr)), std::invalid_argument);
}

namespace {
PFNGLGETSHADERIVPROC shader_status = nullptr;
int shader_checks = 0;
int rejected_shader_check = 0;
PFNGLDELETEPROGRAMPROC delete_program = nullptr;
GL3WProcs* deletion_dispatch = nullptr;
int deleted_programs = 0;
PFNGLDELETETEXTURESPROC delete_textures = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC delete_framebuffers = nullptr;
int deleted_textures = 0;
int deleted_framebuffers = 0;
PFNGLGETUNIFORMLOCATIONPROC uniform_location = nullptr;
const char* rejected_uniform = nullptr;
int uniform_matches = 0;
int rejected_uniform_match = 0;

GLint APIENTRY rejectSelectedUniform(GLuint program, const GLchar* name) {
    if (std::strcmp(name, rejected_uniform) == 0 && ++uniform_matches == rejected_uniform_match) {
        deletion_dispatch = gl3wProcs;
        return -1;
    }

    return uniform_location(program, name);
}

void APIENTRY checkProgramDeletion(GLuint program) {
    EXPECT_EQ(gl3wProcs, deletion_dispatch);
    ++deleted_programs;
    delete_program(program);
}

void APIENTRY checkTextureDeletion(GLsizei count, const GLuint* textures) {
    EXPECT_EQ(gl3wProcs, deletion_dispatch);
    deleted_textures += count;
    delete_textures(count, textures);
}

void APIENTRY checkFramebufferDeletion(GLsizei count, const GLuint* framebuffers) {
    EXPECT_EQ(gl3wProcs, deletion_dispatch);
    deleted_framebuffers += count;
    delete_framebuffers(count, framebuffers);
}

void APIENTRY rejectSelectedShader(GLuint shader, GLenum parameter, GLint* value) {
    if (parameter == GL_COMPILE_STATUS && ++shader_checks == rejected_shader_check) {
        deletion_dispatch = gl3wProcs;
        *value = GL_FALSE;
    } else {
        shader_status(shader, parameter, value);
    }
}
}

TEST(FrameworkWindow, RendererUsesHostDispatchAndCleansUpConstructionFailure) {
    ASSERT_TRUE(glfwInit());
    auto glfw_guard = Volcano::scopeGuard([] {
        glfwTerminate();
    });
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    Volcano::Framework::Window window(32, 32, "Host dispatch lifetime");
    auto* dispatch = gl3wProcs;
    ASSERT_NE(dispatch, nullptr);
    const auto original = *dispatch;
    auto restore = Volcano::scopeGuard([dispatch, original] {
        *dispatch = original;
    });
    shader_status = original.gl.GetShaderiv;
    uniform_location = original.gl.GetUniformLocation;
    delete_program = original.gl.DeleteProgram;
    delete_textures = original.gl.DeleteTextures;
    delete_framebuffers = original.gl.DeleteFramebuffers;
    deletion_dispatch = dispatch;
    dispatch->gl.DeleteProgram = checkProgramDeletion;
    dispatch->gl.DeleteTextures = checkTextureDeletion;
    dispatch->gl.DeleteFramebuffers = checkFramebufferDeletion;

    // Fail every geometry, lighting, transparent and paint shader in turn.
    dispatch->gl.GetShaderiv = rejectSelectedShader;
    for (int check = 1; check <= 8; ++check) {
        deleted_programs = deleted_textures = deleted_framebuffers = 0;
        shader_checks = 0;
        rejected_shader_check = check;
        EXPECT_THROW((Volcano::Graphics::Renderer{}), std::runtime_error);
        EXPECT_EQ(shader_checks, check);
        EXPECT_EQ(deleted_programs, (check + 1) / 2);
        EXPECT_EQ(deleted_textures, 4);
        EXPECT_EQ(deleted_framebuffers, 1);
        EXPECT_EQ(gl3wProcs, dispatch);
    }
    dispatch->gl.GetShaderiv = original.gl.GetShaderiv;

    struct MissingUniform {
        const char* name;
        int occurrence;
        int programs;
    };
    dispatch->gl.GetUniformLocation = rejectSelectedUniform;
    for (const auto& failure : {MissingUniform{"uProjection", 1, 1},
                               MissingUniform{"uAmbient", 1, 2},
                               MissingUniform{"uColor", 2, 3}}) {
        deleted_programs = deleted_textures = deleted_framebuffers = 0;
        rejected_uniform = failure.name;
        rejected_uniform_match = failure.occurrence;
        uniform_matches = 0;
        EXPECT_THROW((Volcano::Graphics::Renderer{}), std::runtime_error);
        EXPECT_EQ(deleted_programs, failure.programs);
        EXPECT_EQ(deleted_textures, 4);
        EXPECT_EQ(deleted_framebuffers, 1);
        EXPECT_EQ(gl3wProcs, dispatch);
    }
    dispatch->gl.GetUniformLocation = original.gl.GetUniformLocation;

    deleted_programs = 0;
    {
        Volcano::Graphics::Renderer renderer;
        EXPECT_EQ(gl3wProcs, dispatch);
        Volcano::Graphics::View view;
        view.setClearColor(Volcano::Colorf::Red);
        view.begin();
        view.end();
        renderer.render(view, window);
        EXPECT_EQ(glGetError(), GL_NO_ERROR);
    }
    EXPECT_EQ(deleted_programs, 4);
    EXPECT_EQ(gl3wProcs, dispatch);
}

TEST(FrameworkWindow, OwnsDispatchAndSwitchesItWithTheWindowContext) {
    ASSERT_TRUE(glfwInit());
    auto glfw_guard = Volcano::scopeGuard([] {
        glfwTerminate();
    });
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    {
        Volcano::Framework::Window first(16, 16, "First context");
        auto* first_dispatch = gl3wProcs;
        ASSERT_NE(first_dispatch, nullptr);
        {
            Volcano::Framework::Window second(16, 16, "Second context");
            auto* second_dispatch = gl3wProcs;
            EXPECT_NE(first_dispatch, second_dispatch);
            EXPECT_EQ(glfwGetCurrentContext(), second.handle());
            first.makeCurrent();
            EXPECT_EQ(gl3wProcs, first_dispatch);
            EXPECT_EQ(glfwGetCurrentContext(), first.handle());
            second.makeCurrent();
            EXPECT_EQ(gl3wProcs, second_dispatch);
            first.makeCurrent();
        }
        EXPECT_EQ(gl3wProcs, first_dispatch);
        EXPECT_EQ(glGetError(), GL_NO_ERROR);
    }
    EXPECT_EQ(gl3wProcs, nullptr);
}

TEST(FrameworkWindow, InitializesGlfwAndKeepsItAliveUntilTheLastWindowCloses) {
    glfwTerminate();
    for (int iteration = 0; iteration < 2; ++iteration) {
        auto first = std::make_unique<Volcano::Framework::Window>(16, 16, "First owner");
        auto* first_dispatch = gl3wProcs;
        EXPECT_EQ(glfwGetCurrentContext(), first->handle());

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        EXPECT_THROW((Volcano::Framework::Window(16, 16, "No GL context")), std::runtime_error);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        EXPECT_EQ(glfwGetCurrentContext(), first->handle());
        EXPECT_EQ(gl3wProcs, first_dispatch);

        auto second = std::make_unique<Volcano::Framework::Window>(16, 16, "Last owner");
        auto* second_dispatch = gl3wProcs;
        first.reset();
        EXPECT_EQ(glfwGetCurrentContext(), second->handle());
        EXPECT_EQ(gl3wProcs, second_dispatch);
        second->makeCurrent();
        EXPECT_EQ(glGetError(), GL_NO_ERROR);
        second.reset();
        EXPECT_EQ(gl3wProcs, nullptr);

        glfwGetError(nullptr);
        EXPECT_EQ(glfwGetCurrentContext(), nullptr);
        EXPECT_EQ(glfwGetError(nullptr), GLFW_NOT_INITIALIZED);
    }
}
