//
//
#include <memory>

#include <argh.h>
#include <SDL_main.h>

#ifdef _WIN32
#   include <spdlog/sinks/msvc_sink.h>
#endif

#include <Volcano/Error.h>
#include <Volcano/ScopeGuard.h>
#include <Volcano/System/Local.h>
#include <Volcano/System/Client.h>
#include <Volcano/System/Server.h>

#include "Volcano/Demo/Common.h"

VOLCANO_DEMO_BEGIN

static void printBanner() {
    spdlog::info("VolcanoDemo v" VOLCANO_VERSION_STR);
}

static void printHelp() {
}

static int run(int argc, char* argv[]) {
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

    spdlog::set_pattern("[%L] %n %t-%o %H:%M:%S.%e: %v");

    printBanner();

    if (cmdline[{ "-v", "--version" }]) {
        return EXIT_SUCCESS;
    }

    if (cmdline[{ "-h", "--help" }]) {
        printHelp();
        return EXIT_SUCCESS;
    }

    std::string mode("local");
    cmdline({ "-m", "--mode" }) >> mode;

    std::string root(std::filesystem::current_path().string());
    cmdline({ "-r", "--root" }) >> root;

    std::string host("127.0.0.1");
    cmdline({ "--host" }) >> host;

    int port = VOLCANO_SYSTEM_DEFAULT_PORT;
    cmdline({ "--port" }) >> port;

    int ret = PHYSFS_init(argv[0]);
    if (!ret) {
        throw Error(Errc::OutOfResource);
    }

    auto physfs_guard = scopeGuard([] {
        PHYSFS_deinit();
    });

    if (!std::filesystem::exists(root)) {
        throw Error(Errc::InvalidParameter);
    }

    ret = PHYSFS_mount(root.c_str(), "/", 0);
    if (!ret) {
        throw Error(Errc::InvalidState);
    }

    const char* wdir = PHYSFS_getPrefDir("Volcano", "Demo");
    if (wdir == nullptr) {
        throw Error(Errc::InvalidContext);
    }

    ret = PHYSFS_setWriteDir(wdir);
    if (!ret) {
        throw Error(Errc::InvalidContext);
    }

    std::unique_ptr<System::Base> system;
    if (mode == "local") {
        logInfo("Creating single player system...");
        system = std::make_unique<System::Local>();
    } else if (mode == "client") {
        logInfo("Creating multi-player client system...");
        system = std::make_unique<System::Client>(host, port);
    } else if (mode == "server") {
        logInfo("Creating multi-player server system...");
        system = std::make_unique<System::Server>(host, port);
    } else {
        logInfo("Invalid mode, fallback to single player system...");
        system = std::make_unique<System::Local>();
    }

    system->run();

    return EXIT_SUCCESS;
}

VOLCANO_DEMO_END


int main(int argc, char* argv[]) {
    int exit_code = EXIT_FAILURE;
    //try {
        exit_code = Volcano::Demo::run(argc, argv);
    //} catch (std::exception& e) {
    //    spdlog::error(e.what());
    //}
    return exit_code;
}
