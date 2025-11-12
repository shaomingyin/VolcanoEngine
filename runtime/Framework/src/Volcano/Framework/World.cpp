//
//
#include <edyn/edyn.hpp>

#include <Volcano/Framework/JsonUtils.h>
#include <Volcano/Framework/World.h>

VOLCANO_FRAMEWORK_BEGIN

World::World(Context& context, const std::filesystem::path& scene_filepath)
    : context_(context)
    , state_(State::Loading) {
    edyn::attach(scene_);
    loading_task_ = async::spawn(async::thread_scheduler(), [this, scene_filepath] {
        // auto config = JsonUtils::loadFromPhysFile("/Config.json");
        // auto perf_dirpath = PHYSFS_getPrefDir(appOrganization().c_str(), appName().c_str());
        // auto user_config_filepath = std::filesystem::path(perf_dirpath) / "VolcanoConfig.h";
        // auto user_config = JsonUtils::loadFromFile(user_config_filepath.c_str());
        // config.merge_patch(user_config);
        // loadConfig(config);
        auto startup_scene = JsonUtils::loadFromPhysFile(scene_filepath.generic_string().c_str());
        loadScene(startup_scene);
    });
}

World::~World() {
    // TODO loading_task_
    edyn::detach(scene_);
}

void World::update(Clock::duration elapsed) noexcept {
    if (state_ == State::Ready) {
        if (physics_enabled_) {
            edyn::update(scene_);
        }
    }
}

void World::loadConfig(const nlohmann::json& config) {
}

void World::loadScene(const nlohmann::json& scene) {
    // TODO load edyn components...
}

VOLCANO_FRAMEWORK_END
