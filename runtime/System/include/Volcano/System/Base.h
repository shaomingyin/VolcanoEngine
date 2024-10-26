//
//
#ifndef VOLCANO_SYSTEM_BASE_H
#define VOLCANO_SYSTEM_BASE_H

#include <filesystem>

#include <Volcano/ResourceManager.h>
#include <Volcano/System/Common.h>
#include <Volcano/System/JsonUtils.h>

VOLCANO_SYSTEM_BEGIN

class Base {
public:
	Base(int sdl_init_flags = SDL_INIT_EVERYTHING,
		 const std::filesystem::path& root = std::filesystem::path(),
		 const std::filesystem::path& init = std::filesystem::path());

	virtual ~Base();

public:
	int fps() const {
		return frame_count_per_second_;
	}

	int maxFps() const {
		return 1000 / durationToMilliseconds(frame_elapsed_min_);
	}

	void setMaxFps(int v) {
		int max_fps = std::clamp(v, 1, 1000);
		frame_elapsed_min_ = std::chrono::microseconds(1000000) / max_fps;
	}

	ResourceManager& resourceManager() {
		return resource_manager_;
	}

	const ResourceManager& resourceManager() const {
		return resource_manager_;
	}

	void run();

protected:
	nlohmann::json& config() {
		return config_;
	}

	const nlohmann::json& config() const {
		return config_;
	}

	template <typename T>
	auto readConfig(const std::filesystem::path& path) const {
		return JsonUtils::read<T>(config_, path);
	}

	template <typename T>
	auto readConfig(const std::filesystem::path& path, T&& def) const {
		return JsonUtils::read<T>(config_, path, std::move(def));
	}

	void stop() {
		running_ = false;
	}

	void pollEvents();

	virtual void mainLoop();
	virtual void handleEvent(const SDL_Event& evnt) {};
	virtual void frame(Duration elapsed) {};

private:
	std::filesystem::path root_;
	std::filesystem::path init_;
	int sdl_init_result_;
	bool running_;
	TimePoint last_frame_timestamp_;
	TimePoint last_frame_count_timestamp_;
	Duration frame_elapsed_min_;
	uint64_t frame_count_;
	uint64_t frame_count_per_second_;
	ResourceManager resource_manager_;
	nlohmann::json config_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_BASE_H
