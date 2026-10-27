#define GLFW_INCLUDE_NONE
#include <stdexcept>
#include <string>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Framework/Window.h>

VOLCANO_FRAMEWORK_BEGIN

Window::Window(int width, int height, const char* title) {
    if (width <= 0 || height <= 0 || title == nullptr) {
        throw std::invalid_argument("Window requires positive dimensions and a title");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    handle_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (handle_ == nullptr) {
        const char* description = nullptr;
        glfwGetError(&description);
        throw std::runtime_error(std::string("Cannot create GLFW window: ") +
            (description != nullptr ? description : "unknown error"));
    }
    auto rollback = scopeGuard([this] { glfwDestroyWindow(handle_); });
    makeCurrent();
    rollback.dismiss();
}

Window::~Window() {
    glfwDestroyWindow(handle_);
}

void Window::makeCurrent() const {
    glfwMakeContextCurrent(handle_);
    if (glfwGetCurrentContext() != handle_) {
        throw std::runtime_error("Cannot activate GLFW OpenGL context");
    }
}

void Window::bind() const noexcept {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int Window::width() const noexcept {
    int width, height;
    glfwGetFramebufferSize(handle_, &width, &height);
    return width;
}

int Window::height() const noexcept {
    int width, height;
    glfwGetFramebufferSize(handle_, &width, &height);
    return height;
}

VOLCANO_FRAMEWORK_END
