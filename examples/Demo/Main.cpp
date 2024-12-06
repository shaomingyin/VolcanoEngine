//
//
#include <format>
#include <memory>

#include <argh.h>
#include <physfs.h>
#include <SDL_main.h>

#ifdef _WIN32
#   include <spdlog/sinks/msvc_sink.h>
#endif

#include <Volcano/ScopeGuard.h>
#include <Volcano/System/Local.h>

static void printHelp() {
}

static void run(int argc, char* argv[]) {
    argh::parser cmdline(argv);

#ifdef VOLCANO_DEBUG
#ifdef _WIN32
    spdlog::set_default_logger(
        std::make_shared<spdlog::logger>("Volcano",
            std::make_shared<spdlog::sinks::msvc_sink_mt>()));
#endif
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif

    if (cmdline[{ "-d", "--debug" }]) {
        spdlog::set_level(spdlog::level::debug);
    }

    spdlog::info("VolcanoDemo v" VOLCANO_VERSION_STR);
    if (cmdline[{ "-v", "--version" }]) {
        return;
    }

    if (cmdline[{ "-h", "--help" }]) {
        printHelp();
        return;
    }

    spdlog::set_pattern("[%L] %n %t-%o %H:%M:%S.%e: %v");

    std::string root(std::filesystem::current_path().string());
    cmdline({ "-r", "--root" }) >> root;
    if (!std::filesystem::exists(root)) {
        throw std::runtime_error(std::format("Invalid root path {}.", root));
    }

    int ret = SDL_Init(SDL_INIT_EVERYTHING);
    if (ret != 0) {
        throw std::runtime_error("Failed to init SDL.");
    }

    auto sdl_guard = Volcano::scopeGuard([] {
        SDL_Quit();
    });

    ret = PHYSFS_init(argv[0]);
    if (!ret) {
        throw std::runtime_error(std::format("Failed to init PhysFS: {}.", PHYSFS_getLastError()));
    }

    auto physfs_guard = Volcano::scopeGuard([] {
        PHYSFS_deinit();
    });

    const char* perf_dir = PHYSFS_getPrefDir("Volcano", "Demo");
    if (perf_dir == nullptr) {
        throw std::runtime_error("Failed to get user dir.");
    }

    Volcano::logInfo("User directory: {}", perf_dir);

    ret = PHYSFS_mount(root.c_str(), "/", 0);
    if (!ret) {
        throw std::runtime_error(std::format("Failed to mount root: {}.", PHYSFS_getLastError()));
    }

    ret = PHYSFS_mount(perf_dir, "/", 0);
    if (!ret) {
        throw std::runtime_error(std::format("Failed to mount user directory {}.", perf_dir));
    }

    ret = PHYSFS_setWriteDir(perf_dir);
    if (!ret) {
        throw std::runtime_error(std::format("Failed to set write dir {}.", perf_dir));
    }

    auto system = std::make_unique<Volcano::System::Local>();
    system->run();
}

int main(int argc, char* argv[]) {
    int exit_code = EXIT_FAILURE;
    //try {
        run(argc, argv);
        exit_code = EXIT_SUCCESS;
    //} catch (std::exception& e) {
    //    spdlog::error(e.what());
    //}
    return exit_code;
}
