//
//
#include <string>
#include <format>
#include <memory>
#include <stdexcept>
#include <filesystem>

#include <argh.h>

#include <Volcano/ScopeGuard.h>
#include <Volcano/FileSystem.h>
#include <Volcano/World/Scene.h>
#include <Volcano/Physics/Common.h>
#include <Volcano/Launcher/Local.h>
#include <Volcano/Launcher/Client.h>
#include <Volcano/Launcher/Common.h>

VOLCANO_LAUNCHER_BEGIN

class Application {
public:
    Application(int argc, char* argv[]) {
        spdlog::set_pattern("%Y-%m-%d %H:%M:%S.%e %t [%L] %v");
        spdlog::info("VolcanoLauncher: Engine version - " VOLCANO_VERSION_STR);
        spdlog::info("Inializing...");

        argh::parser cmdline(argv);

#ifdef VOLCANO_DEBUG
        spdlog::set_level(spdlog::level::debug);
#else
        if (cmdline({ "-d", "--debug" })) {
            spdlog::set_level(spdlog::level::debug);
        } else {
            spdlog::set_level(spdlog::level::warn);
        }
#endif

        std::string root;
        std::string cwd = std::filesystem::current_path().generic_string();
        cmdline({ "-r", "--root" }, cwd) >> root;
        std::filesystem::path root_path(root);

        spdlog::info("Mounting rootfs {}...", root);
        FileSystem::init(argv[0]);
        auto fs_guard = scopeGuard([] {
            FileSystem::shutdown();
        });
        FileSystem::mount("/", root_path.native().c_str());

        Physics::init();
        auto physics_guard = scopeGuard([] {
            Physics::shutdown();
        });

        spdlog::info("Initializing game...");
        auto game = volcanoCreateGame();

        if (cmdline({ "-c", "--client" })) {
            local_ = std::make_unique<Client>(*game);
        } else {
            local_ = std::make_unique<Local>(*game);
        }

        game_ = std::move(game);

        physics_guard.dismiss();
        fs_guard.dismiss();
    }

    ~Application() {
        Physics::shutdown();
        FileSystem::shutdown();
    }

public:
    void run() {
        VOLCANO_ASSERT(local_);
        spdlog::info("Running...");
        local_->run();
    }

private:
    std::unique_ptr<World::Scene> game_;
    std::unique_ptr<Local> local_;
};

VOLCANO_LAUNCHER_END

int main(int argc, char* argv[]) {
    int exit_code;
    try {
        Volcano::Launcher::Application app(argc, argv);
        app.run();
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
