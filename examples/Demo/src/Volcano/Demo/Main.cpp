//
//
#include <memory>

#include <argh.h>
#include <SDL_main.h>

#include <Volcano/ScopeGuard.h>
#include <Volcano/System/SinglePlayer.h>
#include <Volcano/System/MultiPlayerClient.h>
#include <Volcano/System/MultiPlayerServer.h>

#include "Volcano/Demo/Common.h"

VOLCANO_DEMO_BEGIN

static void printBanner() {
    logInfo("VolcanoEngine Demo v" VOLCANO_VERSION_STR);
}

static void printHelp() {
}

static int run(int argc, char* argv[]) {
    argh::parser cmdline(argv);

#ifdef VOLCANO_DEBUG
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#else
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);
#endif

    printBanner();

    if (cmdline[{ "-v", "--version" }]) {
        return EXIT_SUCCESS;
    }

    if (cmdline[{ "-h", "--help" }]) {
        printHelp();
        return EXIT_SUCCESS;
    }

    std::string mode;
    cmdline("mode") >> mode;

    std::unique_ptr<System::Base> system;
    if (mode == "singleplayer") {
        logInfo("Creating single player system...");
        system = std::make_unique<System::SinglePlayer>();
    } else if (mode == "multiplayerclient") {
        logInfo("Creating multi-player client system...");
        system = std::make_unique<System::MultiPlayerClient>();
    } else if (mode == "multiplayerserver") {
        logInfo("Creating multi-player server system...");
        system = std::make_unique<System::MultiPlayerServer>();
    } else {
        if (!mode.empty()) {
            logWarning("Unknown mode: '{}'", mode);
        }
        logWarning("Fallback to 'singleplayer'.");
        system = std::make_unique<System::SinglePlayer>();
    }

    system->run();

    return EXIT_SUCCESS;
}

VOLCANO_DEMO_END

int main(int argc, char* argv[]) {
    return Volcano::Demo::run(argc, argv);
}
