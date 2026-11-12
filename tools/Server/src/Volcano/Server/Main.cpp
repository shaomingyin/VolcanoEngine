#include <chrono>
#include <csignal>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <thread>

#include <argh.h>
#include <physfs.h>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Game.h>
#include <Volcano/Framework/Resources.h>
#include <Volcano/Framework/Server.h>

namespace {
volatile std::sig_atomic_t running = 1;

void requestStop(int) {
    running = 0;
}
}

int main(int argc, char* argv[]) {
    using namespace Volcano;
    try {
        argh::parser cmdline({"-r", "--root", "--ticks"});
        cmdline.parse(argc, argv);
        if (cmdline[{"-r", "--root", "--ticks"}]) {
            throw std::invalid_argument("Missing command-line parameter value");
        }
        const auto root = cmdline({"-r", "--root"}, std::filesystem::current_path().string()).str();
        // A finite tick count allows headless startup checks; zero runs until a signal.
        long long ticks = 0;
        auto tickArgument = cmdline("--ticks", 0);
        if (!(tickArgument >> ticks) || ticks < 0 ||
            (tickArgument >> std::ws).peek() != std::char_traits<char>::eof()) {
            throw std::invalid_argument("--ticks must be a nonnegative integer");
        }
        if (PHYSFS_init(argv[0]) == 0) {
            throw std::runtime_error(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
        }
        auto physfsGuard = scopeGuard([] {
            Framework::Resources::clear();
            PHYSFS_deinit();
        });
        const auto rootPath = std::filesystem::absolute(root).u8string();
        const std::string rootUtf8(rootPath.begin(), rootPath.end());
        if (PHYSFS_mount(rootUtf8.c_str(), nullptr, 1) == 0) {
            throw std::runtime_error(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
        }
        std::signal(SIGINT, requestStop);
        std::signal(SIGTERM, requestStop);
        Framework::Server framework;
        framework.load(volcanoCreateGame);
        framework.play();
        auto last = Clock::now();
        for (long long tick = 0; !framework.shouldClose() && running && (ticks == 0 || tick < ticks); ++tick) {
            const auto now = Clock::now();
            framework.update(now - last);
            last = now;
            if (ticks == 0 || tick + 1 < ticks) {
                std::this_thread::sleep_until(now + std::chrono::milliseconds(16));
            }
        }
        return EXIT_SUCCESS;
    } catch (const std::exception& error) {
        spdlog::error("{}", error.what());
        return EXIT_FAILURE;
    }
}
