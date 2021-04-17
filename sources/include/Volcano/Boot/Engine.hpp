//
//
#ifndef VOLCANO_BOOT_ENGINE_HPP
#define VOLCANO_BOOT_ENGINE_HPP

#include <memory>

#include <GL/gl3w.h>

#include <Volcano/VM/Kernel.hpp>
#include <Volcano/Boot/Common.hpp>

VOLCANO_BOOT_BEGIN

class Engine: public VM::Kernel::Traps {
public:
	Engine(uv_loop_t *loop);
	virtual ~Engine(void);

public:
	bool start(void);
	void stop(void);
	void handleEvent(const SDL_Event &event);

public: // VM::Kernel::Traps
	void output(void) override;

private:
	void handleWindowEvent(const SDL_WindowEvent &event);

private:
	uv_loop_t *m_loop;
	SDL_Window *m_window;
	Uint32 m_windowID;
	int m_windowSize[2];
	bool m_windowVisible;
	SDL_GLContext m_gl;
	union GL3WProcs m_gl3w;
	std::unique_ptr<VM::Kernel> m_vm;
};

VOLCANO_BOOT_END

#endif // VOLCANO_BOOT_ENGINE_HPP
