//
//
#ifndef VOLCANO_LAUNCHER_WINDOW_HPP
#define VOLCANO_LAUNCHER_WINDOW_HPP

#include <string_view>

#include <GL/gl3w.h>
#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

class Window {
public:
	Window(void);
	virtual ~Window(void);

public:
	bool init(void);
	bool handleEvent(const SDL_WindowEvent &evt);
	void update(void);
	Graphics::Renderer &renderer(void);

private:
	SDL_Window *m_window;
	Uint32 m_id;
	int m_size[2];
	bool m_visible;
	SDL_GLContext m_glContext;
	union GL3WProcs m_gl3w;
	Graphics::Renderer m_renderer;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_WINDOW_HPP
