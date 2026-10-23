#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <gtest/gtest.h>
#include <Volcano/ScopeGuard.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Graphics/Texture.h>
#include <Volcano/Launcher/Window.h>
#include <array>
#include <stdexcept>
#include <cstring>

TEST(LauncherWindow, RendersToDefaultFramebufferAndTexture) {
    ASSERT_TRUE(glfwInit());
    auto glfw_guard = Volcano::scopeGuard([] { glfwTerminate(); });
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    Volcano::Launcher::Window window(64, 48, "LauncherWindowCheck");
    EXPECT_EQ(glfwGetCurrentContext(), window.handle());
    int width, height;
    glfwGetFramebufferSize(window.handle(), &width, &height);
    EXPECT_EQ(window.width(), width);
    EXPECT_EQ(window.height(), height);
    ASSERT_GT(width, 0);
    ASSERT_GT(height, 0);

    Volcano::Graphics::Renderer renderer(glfwGetProcAddress);
    Volcano::Graphics::Texture texture(8, 8);
    renderer.begin(texture);
    renderer.clear(Volcano::Math::Colorf::Red);
    renderer.begin(window);
    renderer.clear(Volcano::Math::Colorf::Black);
    renderer.draw(texture);
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

TEST(LauncherWindow, RejectsInvalidArguments) {
    EXPECT_THROW((Volcano::Launcher::Window(0, 48, "invalid")), std::invalid_argument);
    EXPECT_THROW((Volcano::Launcher::Window(64, -1, "invalid")), std::invalid_argument);
    EXPECT_THROW((Volcano::Launcher::Window(64, 48, nullptr)), std::invalid_argument);
}

namespace {
void APIENTRY rejectShader(GLuint, GLenum, GLint* value) {
    *value = 0;
}
}

TEST(LauncherWindow, RendererOwnsDispatchAndRestoresItAfterConstructionFailure) {
    ASSERT_TRUE(glfwInit());
    auto glfw_guard = Volcano::scopeGuard([] { glfwTerminate(); });
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    Volcano::Launcher::Window window(32, 32, "Renderer dispatch lifetime");
    {
        Volcano::Graphics::Renderer renderer(glfwGetProcAddress);
        auto first = gl3wProcs;
        ASSERT_NE(first, nullptr);
        EXPECT_THROW((Volcano::Graphics::Renderer([](const char* name) -> GL3WglProc {
            return std::strcmp(name, "glGetShaderiv") == 0 ?
                reinterpret_cast<GL3WglProc>(rejectShader) : glfwGetProcAddress(name);
        })), std::runtime_error);
        EXPECT_EQ(gl3wProcs, first);
        {
            Volcano::Graphics::Renderer other(glfwGetProcAddress);
            EXPECT_NE(gl3wProcs, first);
            renderer.begin(window);
            EXPECT_EQ(gl3wProcs, first);
        }
        EXPECT_EQ(gl3wProcs, first);
        renderer.clear(Volcano::Math::Colorf::Red);
        EXPECT_EQ(glGetError(), GL_NO_ERROR);
    }
    EXPECT_EQ(gl3wProcs, nullptr);
}
