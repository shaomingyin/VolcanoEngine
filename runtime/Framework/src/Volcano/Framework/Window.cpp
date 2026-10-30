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
    if (instances_ == 0 && !glfwInit()) {
        const char* description = nullptr;
        glfwGetError(&description);
        throw std::runtime_error(std::string("Cannot initialize GLFW: ") +
            (description != nullptr ? description : "unknown error"));
    }
    ++instances_;
    auto runtime_rollback = scopeGuard(&Window::releaseGlfw);

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
    auto* previous_context = glfwGetCurrentContext();
    auto* previous_procs = gl3wProcs;
    auto rollback = scopeGuard([this, previous_context, previous_procs] {
        glfwMakeContextCurrent(previous_context);
        gl3wProcs = previous_procs;
        glfwDestroyWindow(handle_);
    });
    makeCurrent();
    if (gl3wInit(&procs_, glfwGetProcAddress) != GL3W_OK) {
        throw std::runtime_error("Cannot load OpenGL functions from the current window context");
    }
    GLint major = 0, minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    if (major < 3 || (major == 3 && minor < 3)) {
        throw std::runtime_error("Graphics requires OpenGL 3.3 or newer");
    }
    rollback.dismiss();
    runtime_rollback.dismiss();
}

Window::~Window() {
    if (gl3wProcs == &procs_) {
        gl3wProcs = nullptr;
    }
    glfwDestroyWindow(handle_);
    releaseGlfw();
}

void Window::releaseGlfw() noexcept {
    VOLCANO_ASSERT(instances_ > 0);
    if (--instances_ == 0) {
        glfwTerminate();
    }
}

void Window::bind() const noexcept {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::makeCurrent() const {
    glfwMakeContextCurrent(handle_);
    if (glfwGetCurrentContext() != handle_) {
        throw std::runtime_error("Cannot activate GLFW OpenGL context");
    }
    gl3wProcs = &procs_;
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
