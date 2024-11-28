//
//
#include <format>
#include <vector>

#include <Volcano/Demo/Application.h>

VOLCANO_DEMO_BEGIN

Application::Application(std::string manifest_path)
	: player_(scene_) {
#if 0
	auto manifest = nlohmann::parseFromPhysFS(manifest_path);
	current_scene_path_ = nlohmann::parseString(manifest, "entryScene");
#endif
	player_.setFpsMax(60);
}

void Application::run() {
#if 0
	while (!current_scene_path_.empty()) {
		player_.run(current_scene_path_);
		current_scene_path_ = player_.nextPath();
	}
#else
	player_.run("");
#endif
}

VOLCANO_DEMO_END
