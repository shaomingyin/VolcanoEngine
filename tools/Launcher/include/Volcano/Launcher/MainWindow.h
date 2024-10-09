//
//
#ifndef VOLCANO_LAUNCHER_MAINWINDOW_H
#define VOLCANO_LAUNCHER_MAINWINDOW_H

#include <memory>
#include <string>

#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Graphics/OpenGL/Driver.h>
#include <Volcano/Launcher/Common.h>

VOLCANO_LAUNCHER_BEGIN

class MainWindow {
public:
    MainWindow();
    virtual ~MainWindow();

public:
    bool init(const std::string& title, int width, int height);
    bool shouldClose();
    void update(std::chrono::steady_clock::duration elapsed);

private:
    GLFWwindow* handle_;
    std::unique_ptr<Graphics::OpenGL::Driver> graphics_opengl_driver_;
    std::unique_ptr<Graphics::Renderer> graphics_renderer_;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_MAINWINDOW_H
