//
//
#include <Volcano/Error.h>
#include <Volcano/System/Base.h>

VOLCANO_SYSTEM_BEGIN

Base::Base(const std::filesystem::path& root, const std::filesystem::path& init, int sdl_init_flags)
	: root_(root.empty() ? std::filesystem::current_path() : root)
	, init_(init.empty() ? "/init.json" : init)
	, sdl_init_result_(SDL_Init(sdl_init_flags)) {
	if (sdl_init_result_ != 0) {
		throw Error(Errc::OutOfResource);
	}
}

Base::~Base() {
	if (sdl_init_result_ == 0) {
		SDL_Quit();
	}
}

void Base::init() {
	resources_.mount("/", root_);
	auto initrc = resources_.get(init_);
	Buffer json_data(initrc->size());
	if (initrc->load(json_data.data(), json_data.size())) {
		//settings_.nlohmann::json::parse(std::move(json_data));
	}
	setMaxFps(settings_.get<int>("/fpsMax", 60));
}

void Base::run() {
	running_ = true;
	frame_count_ = 0;
	frame_count_per_second_ = 0;
	last_frame_timestamp_ = Clock::now();
	last_frame_count_timestamp_ = last_frame_timestamp_;

	while (running_) {
		pollEvents();
		TimePoint curr = Clock::now();
		auto elapsed = curr - last_frame_timestamp_;
		if (elapsed < frame_elapsed_min_) {
			auto delay = frame_elapsed_min_ - elapsed;
			if (delay > std::chrono::milliseconds(20)) {
				SDL_Delay(20);
			} else {
				SDL_Delay(durationToMilliseconds(delay));
			}
			continue;
		}
		resources_.update();
		settings_.update();
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

void Base::pollEvents() {
	if (SDL_WasInit(SDL_INIT_EVENTS)) {
		SDL_Event evt;
		while (SDL_PollEvent(&evt)) {
			if (evt.type != SDL_QUIT) {
				handleEvent(evt);
			} else {
				running_ = false;
			}
		}
	}
}

VOLCANO_SYSTEM_END
