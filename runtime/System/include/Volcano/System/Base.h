//
//
#ifndef VOLCANO_SYSTEM_BASE_H
#define VOLCANO_SYSTEM_BASE_H

#include <string>
#include <thread>

#include <Volcano/World/Scene.h>
#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

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
	Base(const std::string& manifest_path);
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

	const std::string& manifestPath() const {
		return manifest_path_;
	}

	uint32_t fps() const {
		return frame_count_per_second_;
	}

	uint32_t fpsMax() const {
		return 1000000 / durationToMicroseconds(elapsed_min_);
	}

	void setFpsMax(uint32_t v) {
		uint32_t tmp = std::clamp(v, 1u, 1000u);
		elapsed_min_ = std::chrono::microseconds(1000000 / tmp);
	}

	template <typename T>
	void runTask(T&& t) {
		async::spawn(task_scheduler_, std::move(t)).get();
	}

	template <typename T>
	auto postTask(T&& t) {
		return async::spawn(task_scheduler_, std::move(t));
	}

	void run();

protected:
	State state() const {
		return state_;
	}

protected:
	bool isLoading() const {
		return (state_ == State::Loading);
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

	void cancelLoading() {
		loading_cancellation_.cancel();
	}

	void loadingCancelPoint() {
		try {
			async::interruption_point(loading_cancellation_);
		} catch (async::task_canceled& ex) {
			throw std::runtime_error("Loading is canceled.");
		}
	}

	void loadScene(const std::string& path) {
		loadScene(nlohmann::parseFromPhysFS(path));
	}

	virtual void loadScene(const nlohmann::json& json);
	virtual void loadMap(const nlohmann::json& json, entt::handle map);
	virtual void loadEntity(const nlohmann::json& json, entt::handle entity);

protected:
	bool isReady() const {
		return (state_ == State::Ready);
	}

protected:
	bool isPlaying() const {
		return (state_ == State::Playing);
	}

	void play() {
		if (state_ == State::Ready || state_ == State::Paused || state_ == State::Stopping) {
			state_ = State::Playing;
		}
	}

protected:
	bool isPaused() const {
		return (state_ == State::Paused);
	}

	void pause() {
		if (state_ == State::Playing) {
			state_ = State::Paused;
		}
	}

protected:
	bool isError() const {
		return (state_ == State::Error);
	}

	const std::string& errorMessage() const {
		return error_message_;
	}

	void error(std::string message) {
		state_ = State::Error;
		error_message_ = std::move(message);
	}

protected:
	void stop() {
		if (state_ != State::Idle) {
			state_ = State::Stopping;
		}
	}

	void clear();
	virtual void loadingFrame(Duration elapsed);
	virtual void readyFrame(Duration elapsed);
	virtual void playingFrame(Duration elapsed);
	virtual void pausedFrame(Duration elapsed);
	virtual void errorFrame(Duration elapsed);

private:
	void frame(Duration elapsed);
	void load();
	void loadRigidBody(const nlohmann::json& json, entt::handle entity);

private:
	State state_;
	async::threadpool_scheduler threadpool_;
	async::fifo_scheduler task_scheduler_;
	World::Scene scene_;
	std::string manifest_path_;
	std::string name_;

	Duration elapsed_min_;
	TimePoint frame_last_;
	TimePoint frame_count_last_;
	uint32_t frame_count_;
	uint32_t frame_count_per_second_;

	async::task<void> loading_task_;
	async::cancellation_token loading_cancellation_;
	std::string loading_message_;
	int loading_progress_;

	std::string error_message_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_BASE_H
