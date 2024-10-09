//
//
#include <Volcano/ScopeGuard.h>
#include <Volcano/Launcher/MainWindow.h>

VOLCANO_LAUNCHER_BEGIN

MainWindow::MainWindow()
    : handle_(nullptr) {
}

MainWindow::~MainWindow() {
    if (handle_ != nullptr) {
        glfwDestroyWindow(handle_);
    }
}

bool MainWindow::init(const std::string& title, int width, int height) {
    VOLCANO_ASSERT(handle_ != nullptr);

    //glfwWindowHint()

    handle_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (handle_ == nullptr) {
        return false;
    }

    auto handle_guard = scopeGuard([this] {
        glfwDestroyWindow(handle_);
        handle_ = nullptr;
    });

    glfwShowWindow(handle_);
    glfwMakeContextCurrent(handle_);

    auto graphics_opengl_driver = std::make_unique<Graphics::OpenGL::Driver>();
    if (!graphics_opengl_driver->init()) {
        return false;
    }

    auto graphics_renderer = std::make_unique<Graphics::Renderer>();
    if (!graphics_renderer->init(width, height)) {
        return false;
    }

    graphics_opengl_driver_ = std::move(graphics_opengl_driver);
    graphics_renderer_ = std::move(graphics_renderer);
    graphics_renderer_->setDriver(graphics_opengl_driver_.get());

    return true;
}

bool MainWindow::shouldClose() {
    VOLCANO_ASSERT(handle_ != nullptr);
    return glfwWindowShouldClose(handle_);
}

void MainWindow::update(std::chrono::steady_clock::duration elapsed) {
    VOLCANO_ASSERT(handle_ != nullptr);
    graphics_renderer_->update(elapsed);
}

VOLCANO_LAUNCHER_END
