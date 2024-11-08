//
//
#include <thread>

#include <Volcano/Error.h>
#include <Volcano/ScopeGuard.h>
#include <Volcano/System/Base.h>

VOLCANO_SYSTEM_BEGIN

Base::Base(int sdl_init_flags)
	: sdl_init_result_(SDL_Init(sdl_init_flags))
	, state_(State::Idle)
	, loading_progress_(0) {
	if (sdl_init_result_ != 0) {
		throw Error(Errc::OutOfResource);
	}
	setMaxFps(60);
}

Base::~Base() {
	SDL_Quit();
}

void Base::run() {
	startLoading();

	frame_count_ = 0;
	frame_count_per_second_ = 0;
	last_frame_timestamp_ = Clock::now();
	last_frame_count_timestamp_ = last_frame_timestamp_;

	while (state_ != State::Idle) {
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
		frame(elapsed);
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

void Base::frame(Duration elapsed) {
	if (beginFrame()) {
		auto frame_guard = scopeGuard([this] {
			endFrame();
		});
		scene_.frame(elapsed);
		switch (state_) {
		case State::Playing:
			playingFrame(elapsed);
			break;
		case State::Loading:
			loadingFrame(elapsed);
			break;
		case State::Ready:
			readyFrame(elapsed);
			break;
		case State::Paused:
			pausedFrame(elapsed);
			break;
		case State::Stopping:
			stoppingFrame(elapsed);
			break;
		default:
			errorFrame(elapsed);
			break;
		}
	}
}

void Base::pollEvents() {
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

void Base::loadEntity(World::Entity ent) {
}

void Base::startLoading() {
	if (state_ != State::Idle && state_ != State::Playing && state_ != State::Paused) {
		throw Error(Errc::InvalidState);
	}

	state_ = State::Loading;

	//tf::Taskflow taskflow;
	//taskflow.emplace()

}

VOLCANO_SYSTEM_END
