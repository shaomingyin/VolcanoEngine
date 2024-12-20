//
//
#include <thread>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Net/Local.h>

VOLCANO_NET_BEGIN

Local::Local()
	: window_("Volcano", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600)
	, acoustics_space_(scene())
	, graphics_renderer_(scene())
	, gui_show_(true)
	, show_fps_(true) {
	auto& main_menus = window_.menuBar().menus();
	main_menus.clear();
	auto& file_menu = main_menus.emplace_back("File");
	file_menu.items().emplace_back("New...");
	file_menu.items().emplace_back("Load...");
	file_menu.items().emplace_back("Save...");
	auto& edit_menu = main_menus.emplace_back("Edit");
	edit_menu.items().emplace_back("Undo");
	edit_menu.items().emplace_back("Redo");
	auto& view_menu = main_menus.emplace_back("View");
	view_menu.items().emplace_back("Show Console", console_.visiblility());
	view_menu.items().emplace_back("Show FPS", &show_fps_);
	auto& settings_menu = main_menus.emplace_back("Settings");
	settings_menu.items().emplace_back("Options...");
	auto& help_menu = main_menus.emplace_back("Help");
	help_menu.items().emplace_back("About...");
	console_.hide();
}

void Local::loadConfig(const nlohmann::json& json) {
	Base::loadConfig(json);
	window_.setTitle(nlohmann::parseString(json, "windowTitle", "Untitled"));
	window_.resize(nlohmann::parseVector2i(json, "windowSize", { 800, 600 }));
}

void Local::loadMap(const nlohmann::json& json, entt::handle map) {
	Base::loadMap(json, map);
	runTask([this, &json, map] {

	});
}

void Local::loadEntity(const nlohmann::json& json, entt::handle entity) {
	Base::loadEntity(json, entity);
	runTask([this, &json, entity] {

	});
}

void Local::tick(Duration elapsed) {
	if (!isThreadedSimulation()) {
		pollEvents();
	}

	Base::tick(elapsed);

	switch (state()) {
	case State::Playing:
		break;
	case State::Loading:
		break;
	case State::Paused:
		break;
	case State::Stopping:
		clear();
		break;
	case State::Error:
		break;
	default:
		break;
	}

	graphics_renderer_.update(elapsed);
	acoustics_space_.update(elapsed);
}

void Local::frame(Duration elapsed) {
	if (isThreadedSimulation()) {
		pollEvents();
	}

	Base::frame(elapsed);

	switch (state()) {
	case State::Playing:
		playingFrame(elapsed);
		break;
	case State::Loading:
		loadingFrame(elapsed);
		break;
	case State::Paused:
		pausedFrame(elapsed);
		break;
	case State::Stopping:
		stoppingFrame(elapsed);
		break;
	case State::Error:
		errorFrame(elapsed);
		break;
	default:
		break;
	}

	if (window_.makeCurrent()) {
		graphics_renderer_.render();
		if (show_console_) {
			console_.update();
		}
		showFps();
		window_.swapBuffers();
	}

	acoustics_space_.render();
}

void Local::loadingFrame(Duration elapsed) {
}

void Local::readyFrame(Duration elapsed) {
}

void Local::playingFrame(Duration elapsed) {
}

void Local::pausedFrame(Duration elapsed) {
}

void Local::stoppingFrame(Duration elapsed) {
}

void Local::errorFrame(Duration elapsed) {
}

void Local::handleEvent(const SDL_Event& evt) {
	window_.handleEvent(evt);

	if (evt.type == SDL_KEYDOWN && evt.key.windowID == window_.id()) {
		switch (evt.key.keysym.sym) {
		case SDLK_BACKQUOTE:
			//gui_show_console_ = !gui_show_console_;
			break;
		default:
			break;
		}
	}

	// TODO convert and post event to simulation.
}

void Local::pollEvents() {
	SDL_Event evt;
	while (SDL_PollEvent(&evt)) {
		if (evt.type != SDL_QUIT) {
			handleEvent(evt);
		} else {
			stop();
		}
	}
}

void Local::showFps() {
	if (show_fps_) {
		const auto& display_size = ImGui::GetMainViewport()->Size;
		ImGui::SetNextWindowPos({ display_size.x - 50.0f, display_size.y - 30.0f });
		ImGui::SetNextWindowSize({ 50.0f, 30.0f });
		ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::TextColored({ 0.0f, 1.0f, 0.0f, 1.0f }, "%dfps", fps());
		ImGui::End();
	}
}

VOLCANO_NET_END
