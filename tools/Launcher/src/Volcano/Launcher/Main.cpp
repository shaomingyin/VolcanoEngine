//
//
#include <memory>

#include <argh.h>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Launcher/Common.h>
#include <Volcano/Launcher/Application.h>

VOLCANO_LAUNCHER_BEGIN

static bool parseCommandLine(Application::Options& options, int argc, char* argv[]) {
    argh::parser cmdline(argv);
    // TODO
}

static int main(int argc, char* argv[]) {
    Application::Options options;
    if (!parseCommandLine(options, argc, argv)) {
        return EXIT_FAILURE;
    }

    int ret = glfwInit();
    if (ret < 0) {
        return EXIT_FAILURE;
    }

    auto glfw_guard = Volcano::scopeGuard([] {
        glfwTerminate();
    });

    auto app = std::make_unique<Application>(options);
    if (!app->init("Volcano", 1024, 768)) {
        return EXIT_FAILURE;
    }

    auto last = Clock::now();
    while (!app->shouldQuit()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // TODO
        auto curr = Clock::now();
        auto elapsed = curr - last;
        last = curr;

        glfwPollEvents();
        app->update(elapsed);
    }

    return EXIT_SUCCESS;
}

VOLCANO_LAUNCHER_END

int main(int argc, char* argv[]) {
    return Volcano::Launcher::main(argc, argv);
}
