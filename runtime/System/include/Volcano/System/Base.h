//
//
#ifndef VOLCANO_SYSTEM_BASE_H
#define VOLCANO_SYSTEM_BASE_H

#include <string>
#include <thread>

#include <Volcano/World/Scene.h>
#include <Volcano/System/Common.h>
#include <Volcano/System/Stepper.h>

VOLCANO_SYSTEM_BEGIN

class Base {
public:
	Base();
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

	uint32_t tps() const {
		return ticker_.countPerSecond();
	}

	uint32_t fps() const {
		if (threaded_simulation_) {
			return framer_.countPerSecond();
		}
		return ticker_.countPerSecond();
	}

	uint32_t tpsMax() const {
		return ticker_.max();
	}

	uint32_t fpsMax() const {
		if (threaded_simulation_) {
			return framer_.max();
		}
		return ticker_.max();
	}

	void setTpsMax(uint32_t v) {
		ticker_.setMax(v);
	}

	void setFpsMax(uint32_t v) {
		framer_.setMax(v);
		if (!threaded_simulation_) {
			ticker_.setMax(v);
		}
	}

	template <typename T>
	void runTask(T&& t) {
		postTask(std::move(t)).get();
	}

	template <typename T>
	auto postTask(T&& t) {
		return async::spawn(task_scheduler_, std::move(t));
	}

	void run();

protected:
	enum class State {
		Idle = 0,
		Loading,
		Ready,
		Playing,
		Paused,
		Stopping,
		Error
	};

	State state() const {
		return state_;
	}

	bool isThreadedSimulation() const {
		return threaded_simulation_;
	}

	void stop() {
		if (state_ != State::Idle) {
			state_ = State::Stopping;
		}
	}

	void clear();
	virtual void tick(Duration elapsed);
	virtual void frame(Duration elapsed);

protected:
	bool isLoading() const {
		return (state_ == State::Loading);
	}

	int loadingProgress() const {
		VOLCANO_ASSERT(isLoading());
		return loading_progress_;
	}

	void setLoadingProgress(int v) {
		VOLCANO_ASSERT(isLoading());
		loading_progress_ = std::clamp(v, 0, 100);
	}

	const std::string& loadingMessage() const {
		VOLCANO_ASSERT(isLoading());
		return loading_message_;
	}

	void setLoadingMessage(std::string v) {
		VOLCANO_ASSERT(isLoading());
		loading_message_ = std::move(v);
	}

	void cancelLoading() {
		VOLCANO_ASSERT(isLoading());
		loading_cancellation_.cancel();
	}

	void loadingCancelPoint() {
		VOLCANO_ASSERT(isLoading());
		try {
			async::interruption_point(loading_cancellation_);
		} catch (async::task_canceled& ex) {
			throw std::runtime_error("Loading is canceled.");
		}
	}

	void loadScene(const std::string& path) {
		loadScene(nlohmann::parseFromPhysFS(path));
	}

	virtual void loadConfig(const nlohmann::json & json);
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
	bool isStopping() const {
		return (state_ == State::Stopping);
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

private:
	void load();
	void loadRigidBody(const nlohmann::json& json, entt::handle entity);

private:
	State state_;

	async::fifo_scheduler task_scheduler_;
	bool threaded_simulation_;

	World::Scene scene_;
	std::string name_;

	async::task<void> loading_task_;
	async::cancellation_token loading_cancellation_;
	std::string loading_message_;
	int loading_progress_;

	std::string error_message_;

	Stepper ticker_;
	Stepper framer_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_BASE_H
