//
//
#ifndef VOLCANO_SYSTEM_BASE_H
#define VOLCANO_SYSTEM_BASE_H

#include <string>

#include <Volcano/World/Scene.h>
#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Base {
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
		if (state_ == State::Playing) {
			state_ = State::Stopping;
		}
	}

protected:
	int loadingProgress() const {
		VOLCANO_ASSERT(state_ == State::Loading);
		return loading_progress_;
	}

	const std::string& loadingText() const {
		VOLCANO_ASSERT(state_ == State::Loading);
		return loading_text_;
	}

	World::Scene& scene() {
		return scene_;
	}

	const World::Scene& scene() const {
		return scene_;
	}

	virtual void handleEvent(const SDL_Event& evnt) {}
	virtual bool beginFrame() { return true; }
	virtual void endFrame() {}
	virtual void loadingFrame(Duration elapsed) {}
	virtual void readyFrame(Duration elapsed) {}
	virtual void playingFrame(Duration elapsed) {}
	virtual void pausedFrame(Duration elapsed) {}
	virtual void stoppingFrame(Duration elapsed) {}
	virtual void errorFrame(Duration elapsed) {}
	virtual void loadEntity(World::Entity ent);

private:
	void frame(Duration elapsed);
	void pollEvents();
	void startLoading();

private:
	enum class State {
		Idle = 0,
		Loading,
		Ready,
		Playing,
		Paused,
		Stopping,
		Error
	};

	int sdl_init_result_;
	State state_;
	World::Scene scene_;
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
