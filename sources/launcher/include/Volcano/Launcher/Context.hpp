//
//
#ifndef VOLCANO_LAUNCHER_CONTEXT_HPP
#define VOLCANO_LAUNCHER_CONTEXT_HPP

#include <SDL.h>
#include <GL/gl3w.h>

#include <Volcano/UI/Graphics/Memory.hpp>
#include <Volcano/UI/Graphics/Renderer.hpp>
#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

class Context {
public:
	Context(void);
	virtual ~Context(void);

public:
	bool init(void);
	void handleEvent(const SDL_Event &event);
	void frame(float elapsed);
	bool canQuit(void);

private:
	void handleWindowEvent(const SDL_WindowEvent &evt);

private:
	SDL_Window *m_window;
	Uint32 m_windowId;
	bool m_windowVisible;
	SDL_GLContext m_gl;
	union GL3WProcs m_gl3w;
	UI::Graphics::Memory m_graphicsMemory;
	UI::Graphics::Renderer m_graphicsRenderer;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_CONTEXT_HPP
