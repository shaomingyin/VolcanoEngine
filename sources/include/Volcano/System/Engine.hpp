//
//
#ifndef VOLCANO_SYSTEM_ENGINE_HPP
#define VOLCANO_SYSTEM_ENGINE_HPP

#include <memory>
#include <string_view>

#include <GL/gl3w.h>

#include <Volcano/Graphics/Memory.hpp>
#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Input/Context.hpp>
#include <Volcano/VM/Kernel.hpp>
#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class Engine {
public:
	Engine(uv_loop_t *loop, std::string_view rootPath);
	virtual ~Engine(void);

public:
	bool start(void);
	void stop(void);
	void handleEvent(const SDL_Event &event);

private:
	void handleWindowEvent(const SDL_WindowEvent &event);

private:
	SDL_Window *m_window;
	Uint32 m_windowID;
	int m_windowSize[2];
	bool m_windowVisible;
	SDL_GLContext m_gl;
	union GL3WProcs m_gl3w;
	Graphics::Memory m_graphicsMemory;
	Graphics::Renderer m_graphicsRenderer;
	//Input::Context m_inputContext;
	VM::Kernel m_vmKernel;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_ENGINE_HPP
