//
//
#ifndef VOLCANO_SYSTEM_LOADER_H
#define VOLCANO_SYSTEM_LOADER_H

#include <string>
#include <future>

#include <nlohmann/json.hpp>

#include <Volcano/World/Scene.h>
#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Loader {
public:
	Loader(World::Scene& scene);
	virtual ~Loader();

public:
	void start();

	int progress() const {
		return progress_;
	}

	const std::string& progressText() const {
		return progress_text_;
	}

	bool isCompleted() const {
		return (state_ == State::Completed);
	}

protected:
	void loadManifest(tf::Subflow& subflow);

private:
	enum class State {
		Idle = 0,
		Loading,
		Completed,
		Error
	};

	World::Scene& scene_;
	tf::Executor executor_;
	State state_;
	int progress_;
	std::string progress_text_;
	tf::Future<void> load_future_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_LOADER_H
