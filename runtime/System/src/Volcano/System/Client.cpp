//
//
#include <Volcano/ScopeGuard.h>
#include <Volcano/System/Client.h>

VOLCANO_SYSTEM_BEGIN

Client::Client()
    : window_(nullptr) {
}

Client::~Client() {
    if (window_ != nullptr) {
        glfwMakeContextCurrent(window_);
        graphics_renderer_.reset();
        glfwDestroyWindow(window_);
    }
}

bool Client::init(const std::string& title, int width, int height) {
    VOLCANO_ASSERT(window_ != nullptr);

    //glfwWindowHint()

    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window_ == nullptr) {
        return false;
    }

    auto window_guard = scopeGuard([this] {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    });

    glfwShowWindow(window_);
    glfwMakeContextCurrent(window_);

    auto graphics_renderer = std::make_unique<Graphics::Renderer>();
    if (!graphics_renderer->init(glfwGetProcAddress, width, height)) {
        return false;
    }

    graphics_renderer_ = std::move(graphics_renderer);

    frame_count_ = 0;
    frame_count_per_second_ = 0;

    window_guard.dismiss();

    return true;
}

bool Client::shouldQuit() {
    VOLCANO_ASSERT(window_ != nullptr);
    return glfwWindowShouldClose(window_);
}

void Client::update(Duration elapsed) {
    VOLCANO_ASSERT(window_ != nullptr);
    VOLCANO_ASSERT(graphics_renderer_);

    if (VOLCANO_UNLIKELY(glfwWindowShouldClose(window_))) {
        return;
    }

    frame(elapsed);

    glfwMakeContextCurrent(window_);
    graphics_renderer_->update(elapsed);
    glfwSwapBuffers(window_);

    frame_count_ += 1;

    auto curr = Clock::now();
    if ((curr - last_frame_count_time_point_) >= std::chrono::seconds(1)) {
        frame_count_per_second_ = frame_count_;
        frame_count_ = 0;
        last_frame_count_time_point_ = curr;
    }
}

VOLCANO_SYSTEM_END
