//
//
#include <thread>

#include <Volcano/ScopeGuard.h>
#include <Volcano/System/Local.h>

VOLCANO_SYSTEM_BEGIN

Local::Local()
	: window_("Volcano", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600)
	, acoustics_space_(scene())
	, graphics_renderer_(scene())
	, gui_show_(true)
	, gui_show_console_(false)
	, gui_show_fps_(true)
	, gui_show_demo_(false) {
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
		runGui();
		graphics_renderer_.render();
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

	if (evt.type == SDL_WINDOWEVENT && evt.window.windowID == window_.id()) {
		switch (evt.key.keysym.sym) {
		case SDLK_ESCAPE:
			gui_show_ = !gui_show_;
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

void Local::runGui() {
	if (!gui_show_) {
		return;
	}

	runMainMenuBar();

	if (gui_show_console_) {
		runConsole();
	}

	if (gui_show_demo_) {
		ImGui::ShowDemoWindow(&gui_show_demo_);
	}

	if (gui_show_fps_) {
		runFps();
	}
}

void Local::runMainMenuBar() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			ImGui::MenuItem("New...");
			ImGui::MenuItem("Load...");
			ImGui::MenuItem("Save...");
			ImGui::MenuItem("Quit");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {
			ImGui::MenuItem("Show FPS", "F9", &gui_show_fps_);
			ImGui::MenuItem("Show Console", "F10", &gui_show_console_);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings")) {
			if (ImGui::MenuItem("Options...")) {
				runOptions();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			ImGui::MenuItem("Website...");
			if (ImGui::MenuItem("About...")) {
				runAbout();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Local::runFps() {
	ImGuiViewport* vp = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos({ vp->Size.x - 50, vp->Size.y - 30 });
	ImGui::SetNextWindowSize({ 50, 30 });
	constexpr int flags =
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize;
	if (ImGui::Begin("FPS", nullptr, flags)) {
		ImGui::Text(std::format("{}fps", fps()).c_str());
		ImGui::End();
	}
}

void Local::runConsole() {
	if (ImGui::Begin("Console", &gui_show_console_)) {
		ImGui::End();
	}
}

void Local::runOptions() {
	if (ImGui::Begin("About", nullptr, ImGuiWindowFlags_NoResize)) {
		ImGui::End();
	}
}

void Local::runAbout() {
	if (ImGui::Begin("About", nullptr, ImGuiWindowFlags_NoResize)) {
		ImGui::End();
	}
}

VOLCANO_SYSTEM_END
