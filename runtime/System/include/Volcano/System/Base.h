//
//
#ifndef VOLCANO_SYSTEM_BASE_H
#define VOLCANO_SYSTEM_BASE_H

#include <string>

#include <Volcano/World/Scene.h>
#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Base: public World::Scene {
public:
	Base(int sdl_init_flags = SDL_INIT_EVERYTHING);
	virtual ~Base();

public:
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

	void stop() {
		if (state_ == State::Running) {
			state_ = State::Stopping;
		}
	}

protected:
	enum class State {
		Stopped = 0,
		Loading,
		Running,
		Stopping,
		Error
	};

	int loadingProgress() const {
		return loading_progress_;
	}

	const std::string& loadingText() const {
		return loading_text_;
	}

	void pollEvents();
	void startLoading();
	virtual void handleEvent(const SDL_Event& evnt) {};
	virtual void loadingFrame(Duration elapsed) {}

private:
	int sdl_init_result_;
	State state_;
	TimePoint last_frame_timestamp_;
	TimePoint last_frame_count_timestamp_;
	Duration frame_elapsed_min_;
	uint64_t frame_count_;
	uint64_t frame_count_per_second_;
	int loading_progress_;
	std::string loading_text_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_BASE_H
