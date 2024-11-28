//
//
#include <Volcano/ScopeGuard.h>
#include <Volcano/Player/Window.h>

VOLCANO_PLAYER_BEGIN

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
	handle_guard.dismiss();
	gl_context_guard.dismiss();
}

Window::~Window() {
	SDL_GL_DeleteContext(gl_context_);
	SDL_DestroyWindow(handle_);
}

bool Window::beginFrame() {
	if ((state_ & StateVisible) && (SDL_GL_MakeCurrent(handle_, gl_context_) == 0)) {
		gl3wProcs = &gl3w_;
		return true;
	}
	return false;
}

void Window::endFrame() {
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
			if ((evt.key.keysym.sym == SDLK_RETURN) && (evt.key.keysym.mod & KMOD_ALT)) {
				toggleFullScreen();
			}
			break;
		}
	}
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

VOLCANO_PLAYER_END
