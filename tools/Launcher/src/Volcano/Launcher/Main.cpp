//
//
#include <memory>

#include <argh.h>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Launcher/Common.h>
#include <Volcano/Launcher/MainWindow.h>

VOLCANO_LAUNCHER_BEGIN

struct Options {
    bool threaded = false;
};

static int main(const Options& options) {
    int ret = glfwInit();
    if (ret < 0) {
        return EXIT_FAILURE;
    }

    auto glfw_guard = scopeGuard([] {
        glfwTerminate();
    });

    auto main_window = std::make_unique<MainWindow>();
    if (!main_window->init("Volcano", 1024, 768)) {
        return false;
    }

    while (main_window->shouldClose()) {
        glfwWaitEventsTimeout(0.016666);

        //main_window->update();
    }

    return EXIT_SUCCESS;
}

VOLCANO_LAUNCHER_END

int main(int argc, char* argv[]) {
    Volcano::Launcher::Options options;

    argh::parser cmdline(argv);
    // TODO

    return Volcano::Launcher::main(options);
}
