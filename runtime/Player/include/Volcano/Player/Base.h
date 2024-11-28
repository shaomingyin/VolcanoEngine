//
//
#ifndef VOLCANO_PLAYER_BASE_H
#define VOLCANO_PLAYER_BASE_H

#include <string>

#include <Volcano/World/Scene.h>
#include <Volcano/Player/Common.h>

VOLCANO_PLAYER_BEGIN

class Base {
public:
	enum class State {
		Idle = 0,
		Loading,
		Ready,
		Playing,
		Paused,
		Stopping,
		Error
	};

public:
	Base(World::Scene& scene);
	virtual ~Base() = default;

public:
	const std::string& name() const {
		return name_;
	}

	World::Scene& scene() {
		return scene_;
	}

	const World::Scene& scene() const {
		return scene_;
	}

	const std::string& path() const {
		return path_;
	}

	const std::string& nextPath() const {
		return next_path_;
	}

	uint32_t fpsMax() const {
		return 1000000 / durationToMicroseconds(elapsed_min_);
	}

	void setFpsMax(uint32_t v) {
		uint32_t tmp = std::clamp(v, 1u, 1000u);
		elapsed_min_ = std::chrono::microseconds(1000000 / tmp);
	}

	void run(const std::string& path);

protected:
	State state() const {
		return state_;
	}

	void setNextPath(std::string v) {
		next_path_ = std::move(v);
	}

	int loadingProgress() const {
		return loading_progress_;
	}

	void setLoadingProgress(int v) {
		if (state_ == State::Loading) {
			loading_progress_ = std::clamp(v, 0, 100);
		}
	}

	const std::string& loadingMessage() const {
		return loading_message_;
	}

	void setLoadingMessage(std::string v) {
		if (state_ == State::Loading) {
			loading_message_ = std::move(v);
		}
	}

	void load();
	virtual void loadScene(const nlohmann::json& json);
	virtual void loadEntity(const nlohmann::json& json, entt::handle entity);

	void play() {
		if (state_ == State::Ready || state_ == State::Paused || state_ == State::Stopping) {
			state_ = State::Playing;
		}
	}

	void pause() {
		if (state_ == State::Playing) {
			state_ = State::Paused;
		}
	}

	void stop() {
		if (state_ != State::Idle) {
			state_ = State::Stopping;
		}
	}

	void clear();

	const std::string& errorMessage() const {
		return error_message_;
	}

	void error(std::string message) {
		state_ = State::Error;
		error_message_ = std::move(message);
	}

	virtual void loadingFrame(Duration elapsed);
	virtual void readyFrame(Duration elapsed);
	virtual void playingFrame(Duration elapsed);
	virtual void pausedFrame(Duration elapsed);
	virtual void stoppingFrame(Duration elapsed);
	virtual void errorFrame(Duration elapsed);

private:
	void frame(Duration elapsed);

private:
	World::Scene& scene_;
	std::string path_;
	std::string next_path_;
	std::string name_;
	State state_;

	async::task<void> loading_task_;
	std::string loading_message_;
	int loading_progress_;

	std::string error_message_;

	Duration elapsed_min_;
	TimePoint frame_last_;
	TimePoint frame_count_last_;
	uint32_t frame_count_;
	uint32_t frame_count_per_second_;
};

VOLCANO_PLAYER_END

#endif // VOLCANO_PLAYER_BASE_H
