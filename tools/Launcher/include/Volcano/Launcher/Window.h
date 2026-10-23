#ifndef VOLCANO_LAUNCHER_WINDOW_H
#define VOLCANO_LAUNCHER_WINDOW_H

#include <Volcano/Graphics/Target.h>
#include <Volcano/Launcher/Common.h>

struct GLFWwindow;

VOLCANO_LAUNCHER_BEGIN

// GLFW must be initialized before construction and terminated after destruction.
class Window final : public Graphics::Target {
public:
    Window(int width, int height, const char* title);
    ~Window() override;

    GLFWwindow* handle() const noexcept { return handle_; }
    int width() const noexcept override;
    int height() const noexcept override;

private:
    std::uint32_t framebuffer() const noexcept override { return 0; }
    GLFWwindow* handle_;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_WINDOW_H
