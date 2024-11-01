//
//
#include <memory>

#include <argh.h>
#include <SDL_main.h>

#ifdef _WIN32
#   include <spdlog/sinks/msvc_sink.h>
#endif

#include <Volcano/ScopeGuard.h>
#include <Volcano/System/Local.h>
#include <Volcano/System/Client.h>
#include <Volcano/System/Server.h>

#include "Volcano/Demo/Common.h"

VOLCANO_DEMO_BEGIN

static void printBanner() {
    logInfo("VolcanoEngine Demo v" VOLCANO_VERSION_STR);
}

static void printHelp() {
}

static int run(const argh::parser& cmdline) {
    printBanner();

    if (cmdline[{ "-v", "--version" }]) {
        return EXIT_SUCCESS;
    }

    if (cmdline[{ "-h", "--help" }]) {
        printHelp();
        return EXIT_SUCCESS;
    }

    std::string root(std::filesystem::current_path().string());
    cmdline({ "-r", "--root" }) >> root;

    std::string init("/init.json");
    cmdline({ "-i", "--init" }) >> init;

    std::string mode;
    cmdline({ "-m", "--mode" }) >> mode;

    std::unique_ptr<System::Base> system;
    if (mode == "local") {
        logInfo("Creating single player system...");
        system = std::make_unique<System::Local>(root, init);
    } else if (mode == "client") {
        std::string remote_host("127.0.0.1");
        cmdline("--remote-host") >> remote_host;
        int remote_port = VOLCANO_SYSTEM_DEFAULT_PORT;
        cmdline("--remote-port") >> remote_port;
        logInfo("Creating multi-player client system...");
        system = std::make_unique<System::Client>(root, init, remote_host, remote_port);
    } else if (mode == "server") {
        std::string bind_host("any");
        cmdline("--bind-host") >> bind_host;
        int bind_port = VOLCANO_SYSTEM_DEFAULT_PORT;
        cmdline("--bind-port") >> bind_port;
        logInfo("Creating multi-player server system...");
        system = std::make_unique<System::Server>(root, init, bind_host, bind_port);
    } else {
        if (!mode.empty()) {
            logWarning("Unknown mode: '{}'", mode);
        }
        logWarning("Fallback to 'local'.");
        system = std::make_unique<System::Local>(root, init);
    }

    system->run();

    return EXIT_SUCCESS;
}

VOLCANO_DEMO_END

int main(int argc, char* argv[]) {
    int exit_code = EXIT_FAILURE;

    try {
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

        spdlog::set_pattern("[%L] %n %t-%o %H:%M:%S.%e: %v");

        exit_code = Volcano::Demo::run(cmdline);
    } catch (std::exception& e) {
        spdlog::error(e.what());
    }

    return exit_code;
}
