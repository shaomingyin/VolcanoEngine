//
//
#include <thread>

#include <Volcano/Error.h>
#include <Volcano/ScopeGuard.h>
#include <Volcano/System/Player.h>

VOLCANO_SYSTEM_BEGIN

Player::Player(World::Scene& scene)
	: scene_(scene)
	, state_(State::Stopped)
	, loading_progress_(0) {
	setMaxFps(60);
}

void Player::run() {
	if (state_ != State::Stopped) {
		throw Error(Errc::InvalidState);
	}

	startLoading();

	frame_count_ = 0;
	frame_count_per_second_ = 0;
	last_frame_timestamp_ = Clock::now();
	last_frame_count_timestamp_ = last_frame_timestamp_;
	bool running = true;

	while (running) {
		pollEvents();

		TimePoint curr = Clock::now();
		auto elapsed = curr - last_frame_timestamp_;
		if (elapsed < frame_elapsed_min_) {
			auto delay = frame_elapsed_min_ - elapsed;
			if (delay > 20ms) {
				std::this_thread::sleep_for(20ms);
			} else {
				std::this_thread::sleep_for(delay);
			}
			continue;
		}

		switch (state_) {
		case State::Running:
			//frame(elapsed);
			break;
		case State::Loading:
			loadingFrame(elapsed);
			break;
		default:
			running = false;
			break;
		}

		frame_count_ += 1;
		last_frame_timestamp_ = curr;

		if ((curr - last_frame_count_timestamp_) >= std::chrono::seconds(1)) {
			frame_count_per_second_ = frame_count_;
			frame_count_ = 0;
			last_frame_count_timestamp_ = curr;
			logDebug("FPS: {}", frame_count_per_second_);
		}
	}
}

void Player::pollEvents() {
	if (SDL_WasInit(SDL_INIT_EVENTS)) {
		SDL_Event evt;
		while (SDL_PollEvent(&evt)) {
			if (evt.type != SDL_QUIT) {
				handleEvent(evt);
			} else {
				state_ = State::Stopping;
			}
		}
	}
}

void Player::startLoading() {
	state_ = State::Loading;
	loading_progress_ = 0;
	loading_text_ = "Initializing...";

	tf::Taskflow taskflow;
	//taskflow.emplace()
}

VOLCANO_SYSTEM_END
