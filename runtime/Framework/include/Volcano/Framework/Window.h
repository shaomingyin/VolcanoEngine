#ifndef VOLCANO_FRAMEWORK_WINDOW_H
#define VOLCANO_FRAMEWORK_WINDOW_H

#include <cstddef>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <Volcano/Graphics/Target.h>
#include <Volcano/Framework/Common.h>

VOLCANO_FRAMEWORK_BEGIN

// Owns GLFW initialization, window context and GL3W; use on the main thread.
class Window final : public Graphics::Target {
public:
    Window(int width, int height, const char* title);
    ~Window() override;

    GLFWwindow* handle() const noexcept {
        return handle_;
    }

    void makeCurrent() const;
    void bind() const noexcept override;
    int width() const noexcept override;
    int height() const noexcept override;

private:
    static void releaseGlfw() noexcept;

private:
    static inline std::size_t instances_ = 0;
    GLFWwindow* handle_;
    mutable GL3WProcs procs_{};
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_WINDOW_H
