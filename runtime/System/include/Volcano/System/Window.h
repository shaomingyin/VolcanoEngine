//
//
#ifndef VOLCANO_SYSTEM_WINDOW_H
#define VOLCANO_SYSTEM_WINDOW_H

#include <string>

#include <GL/gl3w.h>

#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Window {
public:
	Window(const std::string& title, int width, int height);
	virtual ~Window();

public:
	void handleEvent(const SDL_Event& evt);

	Uint32 id() const {
		return id_;
	}

	bool isVisible() const {
		return (flags_ & FlagVisible);
	}

	void show() {
		VOLCANO_ASSERT(handle_ != nullptr);
		SDL_ShowWindow(handle_);
	}

	void hide() {
		VOLCANO_ASSERT(handle_ != nullptr);
		SDL_HideWindow(handle_);
	}

	int width() const {
		VOLCANO_ASSERT(handle_ != nullptr);
		int w, h;
		SDL_GetWindowSize(handle_, &w, &h);
		return w;
	}

	int height() const {
		VOLCANO_ASSERT(handle_ != nullptr);
		int w, h;
		SDL_GetWindowSize(handle_, &w, &h);
		return h;
	}

	bool makeCurrent() {
		VOLCANO_ASSERT(handle_ != nullptr);
		VOLCANO_ASSERT(gl_context_ != nullptr);
		if (SDL_GL_MakeCurrent(handle_, gl_context_) == 0) {
			gl3wProcs = &gl3w_;
			return true;
		}
		return false;
	}

	bool beginDraw() {
		VOLCANO_ASSERT(handle_ != nullptr);
		VOLCANO_ASSERT(gl_context_ != nullptr);
		if ((flags_ & FlagVisible) && (SDL_GL_MakeCurrent(handle_, gl_context_) == 0)) {
			gl3wProcs = &gl3w_;
			return true;
		}
		return false;
	}

	void endDraw() {
		VOLCANO_ASSERT(handle_ != nullptr);
		SDL_GL_SwapWindow(handle_);
		gl3wProcs = nullptr;
	}

private:
	enum {
		FlagVisible = 0x1
	};

	int flags_;
	SDL_Window* handle_;
	Uint32 id_;
	SDL_GLContext gl_context_;
	GL3WProcs gl3w_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_WINDOW_H
