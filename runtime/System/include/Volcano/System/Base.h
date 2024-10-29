//
//
#ifndef VOLCANO_SYSTEM_BASE_H
#define VOLCANO_SYSTEM_BASE_H

#include <filesystem>

#include <Volcano/Resources.h>
#include <Volcano/World/Scene.h>
#include <Volcano/System/Common.h>
#include <Volcano/System/Settings.h>

VOLCANO_SYSTEM_BEGIN

class Base: public World::Scene {
public:
	Base(const std::filesystem::path& root = std::filesystem::path(),
		 const std::filesystem::path& init = std::filesystem::path(),
		 int sdl_init_flags = SDL_INIT_EVERYTHING);

	virtual ~Base();

public:
	virtual void init();
	void run();

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

	Resources& resources() {
		return resources_;
	}

	const Resources& resources() const {
		return resources_;
	}

	Settings& settings() {
		return settings_;
	}

	const Settings& settings() const {
		return settings_;
	}

	void stop() {
		running_ = false;
	}

protected:
	void pollEvents();
	virtual void handleEvent(const SDL_Event& evnt) {};

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
	Settings settings_;
	Resources resources_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_BASE_H
