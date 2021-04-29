//
//
#ifndef VOLCANO_SYSTEM_ENGINE_HPP
#define VOLCANO_SYSTEM_ENGINE_HPP

#include <memory>

#include <GL/gl3w.h>

#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Input/Context.hpp>
#include <Volcano/VM/Kernel.hpp>
#include <Volcano/System/Common.hpp>
#include <Volcano/System/Window.hpp>
#include <Volcano/System/Sound.hpp>

VOLCANO_SYSTEM_BEGIN

class Engine: public VM::Kernel::Traps {
public:
	Engine(uv_loop_t *loop);
	~Engine(void) override;

public:
	bool init(void);
	void handleEvent(const SDL_Event &event);

public: // vm traps
	VM::Window *window(void) override;
	VM::Sound *sound(void) override;

private:
	Window m_window;
	Sound m_sound;
	//Input::Context m_inputContext;
	VM::Kernel m_vm;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_ENGINE_HPP
