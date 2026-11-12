#define GLFW_INCLUDE_NONE
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>

#include <argh.h>
#include <GLFW/glfw3.h>
#include <physfs.h>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Game.h>
#include <Volcano/Framework/Resources.h>
#include <Volcano/Framework/Client.h>
#include <Volcano/Framework/Local.h>
#include <Volcano/World/OrthographicCamera.h>
#include <Volcano/World/PerspectiveCamera.h>

int main(int argc, char* argv[]) {
    using namespace Volcano;
    try {
        argh::parser cmdline({"-r", "--root"});
        cmdline.parse(argc, argv);
        if (cmdline[{"-r", "--root"}]) {
            throw std::invalid_argument("Missing command-line parameter value");
        }
        spdlog::set_pattern("%Y-%m-%d %H:%M:%S.%e %t [%L] %v");
#ifdef VOLCANO_DEBUG
        spdlog::set_level(spdlog::level::debug);
#else
        spdlog::set_level(cmdline[{"-d", "--debug"}] ? spdlog::level::debug : spdlog::level::warn);
#endif
        if (PHYSFS_init(argv[0]) == 0) {
            throw std::runtime_error(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
        }
        auto physfsGuard = scopeGuard([] {
            Framework::Resources::clear();
            PHYSFS_deinit();
        });
        const auto root = cmdline({"-r", "--root"}, std::filesystem::current_path().string()).str();
        const auto rootPath = std::filesystem::absolute(root).u8string();
        const std::string rootUtf8(rootPath.begin(), rootPath.end());
        if (PHYSFS_mount(rootUtf8.c_str(), nullptr, 1) == 0) {
            throw std::runtime_error(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
        }
        std::unique_ptr<Framework::Local> framework;
        if (cmdline[{"-c", "--client"}]) {
            framework = std::make_unique<Framework::Client>(800, 600, "VolcanoLauncher");
        } else {
            framework = std::make_unique<Framework::Local>(800, 600, "VolcanoLauncher");
        }
        auto& window = framework->window();
        framework->loadAsync(volcanoCreateGame);
        glfwSwapInterval(1);

        auto last = Clock::now();
        while (!framework->shouldClose()) {
            glfwPollEvents();
            if (framework->shouldClose()) {
                break;
            }
            const auto now = Clock::now();
            framework->update(now - last);
            last = now;
            if (framework->state() == Framework::Base::State::Loading) {
                const auto& progress = framework->loadingProgress();
                const auto title = "VolcanoLauncher — Loading " +
                    std::to_string(static_cast<int>(progress.percent)) + "% " + progress.content;
                glfwSetWindowTitle(window.handle(), title.c_str());
                glfwWaitEventsTimeout(0.01);
                continue;
            }
            if (framework->state() == Framework::Base::State::Ready) {
                framework->play();
                glfwSetWindowTitle(window.handle(), "VolcanoLauncher");
            }
            if (window.width() == 0 || window.height() == 0) {
                glfwWaitEventsTimeout(0.1);
                last = Clock::now();
                continue;
            }

            auto& scene = framework->scene();
            entt::entity camera = entt::null;
            for (const auto entity : scene.view<World::PerspectiveCamera>()) {
                camera = entity;
                scene.get<World::PerspectiveCamera>(entity).setAspect(
                    static_cast<float>(window.width()) / static_cast<float>(window.height()));
                break;
            }
            if (camera == entt::null) {
                for (const auto entity : scene.view<World::OrthographicCamera>()) {
                    camera = entity;
                    break;
                }
            }
            if (camera == entt::null) {
                throw std::runtime_error("Scene requires a camera for Launcher rendering");
            }
            framework->render(camera);
            glfwSwapBuffers(window.handle());
        }
        return EXIT_SUCCESS;
    } catch (const std::exception& error) {
        spdlog::error("{}", error.what());
        return EXIT_FAILURE;
    }
}
