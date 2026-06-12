//
//
#include <string>
#include <format>
#include <memory>
#include <stdexcept>
#include <filesystem>

#include <argh.h>
#include <physfs.h>
#include <rttr/type>

#include <Volcano/ScopeGuard.h>
#include <Volcano/World/Scene.h>
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

        spdlog::info("Initializing physfs...");
        int ret = PHYSFS_init(argv[0]);
        if (!ret) {
            throw std::runtime_error(std::format("Failed to init PHYSFS: {}",
                PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
        }
        auto physfs_guard = scopeGuard([] {
            PHYSFS_deinit();
        });

        std::string root;
        std::string cwd = std::filesystem::current_path().generic_string();
        cmdline({ "-r", "--root" }, cwd) >> root;

        spdlog::info("Mounting rootfs {}...", root);
        ret = PHYSFS_mount(root.c_str(), "/", 1);
        if (!ret) {
            auto ec = PHYSFS_getLastErrorCode();
            throw std::runtime_error(std::format("Failed to mount rootfs: {}", PHYSFS_getErrorByCode(ec)));
        }

        //Physics::init();

        spdlog::info("Setup world...");
        scene_ = createVolcanoWorldScene();
        spdlog::info("World: {} - {}", scene_->name(), scene_->version().toString());

        if (cmdline({ "-c", "--client" })) {
            local_ = std::make_unique<Client>(*scene_);
        } else {
            local_ = std::make_unique<Local>(*scene_);
        }

        physfs_guard.dismiss();
    }

    ~Application() {
        PHYSFS_deinit();
    }

public:
    void run() {
        VOLCANO_ASSERT(local_);
        spdlog::info("Running...");
        local_->run();
    }

private:
    std::unique_ptr<World::Scene> scene_;
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
