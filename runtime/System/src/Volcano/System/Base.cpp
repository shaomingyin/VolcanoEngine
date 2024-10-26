//
//
#include <Volcano/System/Base.h>

VOLCANO_SYSTEM_BEGIN

Base::Base(int sdl_init_flags, const std::filesystem::path& root, const std::filesystem::path& init)
	: root_(root.empty() ? std::filesystem::current_path() : root)
	, init_(init.empty() ? "/init.json" : init)
	, sdl_init_result_(SDL_Init(sdl_init_flags)) {
	resource_manager_.mount("/", root_);
}

Base::~Base() {
	if (sdl_init_result_) {
		SDL_Quit();
	}
}

void Base::run() {
	auto initrc = resource_manager_.get(init_);
	Buffer json_data(initrc->size());
	if (initrc->load(json_data.data(), json_data.size())) {
		config_ = nlohmann::json::parse(std::move(json_data));
	}

	setMaxFps(readConfig<int>("/fpsMax", 60));

	running_ = true;
	frame_count_ = 0;
	frame_count_per_second_ = 0;
	last_frame_timestamp_ = Clock::now();
	last_frame_count_timestamp_ = last_frame_timestamp_;

	mainLoop();
}

void Base::mainLoop() {
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
		frame(elapsed);
		resource_manager_.update();
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
