//
//
#include <memory>

#include <argh.h>

#include <Volcano/Framework/Local.h>
#include <Volcano/Framework/Client.h>
#include <Volcano/Launcher/Common.h>

VOLCANO_LAUNCHER_BEGIN

static void run(const argh::parser& cmdline) {
    logInfo("VolcanoLauncher: Engine version - " VOLCANO_VERSION_STR);
    logInfo("Inializing...");
    std::unique_ptr<Framework::Local> app;
    if (true) {
        app = std::make_unique<Framework::Local>("Demo");
    } else {
        app = std::make_unique<Framework::Client>("Demo");
    }
    logInfo("Running...");
    app->run();
}

VOLCANO_LAUNCHER_END

int main(int argc, char* argv[]) {
    int exit_code;
    try {
        spdlog::set_pattern("%Y-%m-%d %H:%M:%S.%e %t [%L] %v");
#ifdef VOLCANO_DEBUG
        spdlog::set_level(spdlog::level::debug);
#else
        spdlog::set_level(spdlog::level::warn);
#endif
        argh::parser cmdline(argv);
        Volcano::Launcher::run(cmdline);
        exit_code = EXIT_SUCCESS;
    } catch (const std::exception& e) {
        spdlog::error(e.what());
        exit_code = EXIT_FAILURE;
    } catch (...) {
        spdlog::error("Unknown error.");
        exit_code = EXIT_FAILURE;
    }
    return exit_code;
}
