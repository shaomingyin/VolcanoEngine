//
//
#include <string>
#include <format>
#include <memory>
#include <stdexcept>

#include <argh.h>
#include <physfs.h>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Framework/Local.h>
#include <Volcano/Framework/Client.h>
#include <Volcano/Launcher/Common.h>

VOLCANO_LAUNCHER_BEGIN

static void run(int argc, char* argv[]) {
    logInfo("VolcanoLauncher: Engine version - " VOLCANO_VERSION_STR);

    argh::parser cmdline(argv);

    logInfo("Inializing...");

    spdlog::set_pattern("%Y-%m-%d %H:%M:%S.%e %t [%L] %v");
#ifdef VOLCANO_DEBUG
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::warn);
#endif

    int ret = PHYSFS_init(argv[0]);
    if (!ret) {
        throw std::runtime_error(std::format("Failed to init PHYSFS: {}",
            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
    }
    auto physfs_guard = scopeGuard([] {
        PHYSFS_deinit();
    });

    std::string root;
    cmdline({ "-r", PHYSFS_getBaseDir() }) >> root;

    ret = PHYSFS_mount(root.c_str(), "/", 1);

    std::unique_ptr<Framework::Local> app;
    if (true) {
        app = std::make_unique<Framework::Local>(nullptr);
    } else {
        app = std::make_unique<Framework::Client>(nullptr);
    }

    logInfo("Running...");
    app->run();
}

VOLCANO_LAUNCHER_END

int main(int argc, char* argv[]) {
    int exit_code;
    try {
        Volcano::Launcher::run(argc, argv);
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
