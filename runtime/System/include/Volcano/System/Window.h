//
//
#ifndef VOLCANO_SYSTEM_WINDOW_H
#define VOLCANO_SYSTEM_WINDOW_H

#include <string>

#include <Volcano/Graphics/Target.h>
#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Window: public Graphics::Target {
public:
	enum {
		FlagResizable = 0x1,
		FlagClear = 0x2
	};

public:
	Window(const std::string& title, int width, int height, int flags = 0);
	virtual ~Window();

public:
	void handleEvent(const SDL_Event& evt);
	bool beginFrame() override;
	void endFrame() override;
	void resize(int width, int height) override;

	SDL_Window* handle() {
		return handle_;
	}

	Uint32 id() const {
		return id_;
	}

	SDL_GLContext glContext() {
		return gl_context_;
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
		return width_;
	}

	int height() const {
		return height_;
	}

private:
	// Internal flags
	enum {
		FlagVisible = 0x1000,
		FlagDrawing = 0x2000
	};

private:
	int flags_;
	int width_;
	int height_;
	SDL_Window* handle_;
	Uint32 id_;
	SDL_GLContext gl_context_;
	GL3WProcs gl3w_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_WINDOW_H
