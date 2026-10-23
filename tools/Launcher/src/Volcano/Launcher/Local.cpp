//
//
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>
#include <thread>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Launcher/Window.h>
#include <Volcano/Launcher/Local.h>

VOLCANO_LAUNCHER_BEGIN

Local::Local(World::Scene& scene)
    : scene_(scene)
    , elapsed_min_(std::chrono::nanoseconds(1'000'000'000 / 60))
    , frame_last_(Clock::now())
    , frame_count_last_(frame_last_)
    , frame_count_(0)
    , frame_count_per_second_(0) {
}

void Local::run() {
    if (!glfwInit()) {
        const char* description = nullptr;
        glfwGetError(&description);
        throw std::runtime_error(std::string("Cannot initialize GLFW: ") +
            (description != nullptr ? description : "unknown error"));
    }
    auto glfw_guard = scopeGuard([] { glfwTerminate(); });
    Window window(800, 600, "VolcanoLauncher");
    Graphics::Renderer renderer(glfwGetProcAddress);
    glfwSwapInterval(1);

    frame_last_ = frame_count_last_ = Clock::now();
    frame_count_ = frame_count_per_second_ = 0;
    while (!glfwWindowShouldClose(window.handle())) {
        glfwPollEvents();
        if (glfwWindowShouldClose(window.handle())) break;
        if (window.width() == 0 || window.height() == 0) {
            glfwWaitEventsTimeout(0.1);
            frame_last_ = Clock::now();
            continue;
        }
        scheduler_.run_all_tasks();
        const auto now = Clock::now();
        frame(now - frame_last_);
        renderer.begin(window);
        renderer.clear();
        glfwSwapBuffers(window.handle());
        ++frame_count_;
        if (now - frame_count_last_ >= std::chrono::seconds(1)) {
            frame_count_per_second_ = static_cast<unsigned long>(
                frame_count_ / std::chrono::duration<double>(now - frame_count_last_).count());
            frame_count_ = 0;
            frame_count_last_ = now;
        }
        frame_last_ = now;
        std::this_thread::sleep_until(now + elapsed_min_);
    }
    // Destruction order keeps the Context alive until renderer releases its GPU objects.
}

unsigned long Local::fps() const noexcept {
    return frame_count_per_second_;
}

unsigned long Local::fpsMax() const noexcept {
    return fps_max_;
}

void Local::setFpsMax(unsigned long v) noexcept {
    fps_max_ = v;
    elapsed_min_ = v == 0 ? Clock::duration::zero() :
        std::chrono::duration_cast<Clock::duration>(std::chrono::duration<double>(1.0 / v));
}

void Local::frame(Clock::duration elapsed) {
    scene_.update(elapsed);
}

VOLCANO_LAUNCHER_END
