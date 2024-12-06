//
//
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include <Volcano/ScopeGuard.h>
#include <Volcano/System/Window.h>

VOLCANO_SYSTEM_BEGIN

Window::Window(const std::string& title, int x, int y, int w, int h)
	: Graphics::Target(w, h)
	, state_(StateVisible)
	, handle_(SDL_CreateWindow(title.c_str(), x, y, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI)) {
	if (handle_ == nullptr) {
		throw std::runtime_error("Failed to create SDL window.");
	}

	auto handle_guard = scopeGuard([this] {
		SDL_DestroyWindow(handle_);
		handle_ = nullptr;
	});

	id_ = SDL_GetWindowID(handle_);
	SDL_GetWindowPosition(handle_, &x_, &y_);
	gl_context_ = SDL_GL_CreateContext(handle_);
	if (gl_context_ == nullptr) {
		throw std::runtime_error("Failed to create OpenGL context.");
	}

	auto gl_context_guard = scopeGuard([this] {
		SDL_GL_DeleteContext(gl_context_);
		gl_context_ = nullptr;
	});

	if (SDL_GL_MakeCurrent(handle_, gl_context_) != 0) {
		throw std::runtime_error("Failed to make current OpenGL context.");
	}

	if (gl3wInit(&gl3w_, GL3WGetProcAddressProc(SDL_GL_GetProcAddress)) != GL3W_OK) {
		throw std::runtime_error("Failed to initialize GL3W.");
	}
	gl3wProcs = &gl3w_;

	imgui_ = ImGui::CreateContext();
	auto imgui_guard = scopeGuard([this] {
		ImGui::DestroyContext(imgui_);
	});

	ImGui::SetCurrentContext(imgui_);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	if (!ImGui_ImplSDL2_InitForOpenGL(handle_, gl_context_)) {
		throw std::runtime_error("Failed to init imgui SDL2 backend.");
	}

	if (!ImGui_ImplOpenGL3_Init("#version 130")) {
		throw std::runtime_error("Failed to init imgui OpenGL3 backend.");
	}

	handle_guard.dismiss();
	gl_context_guard.dismiss();
	imgui_guard.dismiss();
}

Window::~Window() {
	SDL_GL_MakeCurrent(handle_, gl_context_);

	const GL3WProcs* bak = &gl3w_;
	std::swap(gl3wProcs, bak);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext(imgui_);
	std::swap(gl3wProcs, bak);

	SDL_GL_DeleteContext(gl_context_);
	SDL_DestroyWindow(handle_);
}

void Window::resize(int w, int h) {
	SDL_SetWindowSize(handle_, w, h);
}

bool Window::makeCurrent() {
	if ((state_ & StateVisible) && (SDL_GL_MakeCurrent(handle_, gl_context_) == 0)) {
		gl3wProcs = &gl3w_;
		ImGui::SetCurrentContext(imgui_);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		return true;
	}
	return false;
}

void Window::swapBuffers() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(handle_);
}

void Window::handleEvent(const SDL_Event& evt) {
	if (evt.type == SDL_WINDOWEVENT && evt.window.windowID == id_) {
		switch (evt.window.event) {
		case SDL_WINDOWEVENT_SHOWN:
			state_ |= StateVisible;
			break;
		case SDL_WINDOWEVENT_HIDDEN:
			state_ &= ~StateVisible;
			break;
		case SDL_WINDOWEVENT_MOVED:
			x_ = evt.window.data1;
			y_ = evt.window.data2;
			break;
		case SDL_WINDOWEVENT_RESIZED:
			Graphics::Target::resize(evt.window.data1, evt.window.data2);
			break;
		case SDL_KEYDOWN:
			handleKeyEvent(evt);
			break;
		default:
			break;
		}
	}

	ImGui_ImplSDL2_ProcessEvent(&evt);
}

void Window::toggleFullScreen() {
	if (state_ & StateFullScreen) {
		if (SDL_SetWindowFullscreen(handle_, SDL_WINDOW_FULLSCREEN) == 0) {
			state_ &= ~StateFullScreen;
		}
	} else {
		if (SDL_SetWindowFullscreen(handle_, 0) == 0) {
			state_ |= StateFullScreen;
		}
	}
}

void Window::handleKeyEvent(const SDL_Event& evt) {
	switch (evt.key.keysym.sym) {
	case SDLK_RETURN:
		if (evt.key.keysym.mod & KMOD_ALT) {
			toggleFullScreen();
		}
		break;
	default:
		break;
	}
}

VOLCANO_SYSTEM_END
