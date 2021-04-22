//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/System/Engine.hpp>

VOLCANO_SYSTEM_BEGIN

Engine::Engine(uv_loop_t *loop):
	m_vm(loop)
{
	VOLCANO_ASSERT(loop != nullptr);
}

Engine::~Engine(void)
{
}

bool Engine::start(std::string_view rootPath)
{
	VOLCANO_LOGI("Initializing window...");
	bool ret = m_window.init("Volcano", 800, 600);
	if (!ret) {
		VOLCANO_LOGE("Failed to initialize OpenGL window.");
		return false;
	}

	ScopeGuard windowGuard([this] { m_window.shutdown(); });

	VOLCANO_LOGI("Starting VM...");
	if (!m_vm.start(rootPath, this)) {
		VOLCANO_LOGE("Failed to start VM.");
		return false;
	}

	windowGuard.dismiss();

	return true;
}

void Engine::stop(void)
{
	VOLCANO_LOGI("Stopping VM...");
    m_vm.stop();

	VOLCANO_LOGI("Shuting down window...");
	m_window.shutdown();
}

void Engine::handleEvent(const SDL_Event &event)
{
    switch (event.type) {
    case SDL_WINDOWEVENT:
        m_window.handleEvent(event.window);
        break;
    default:
        m_vm.postEvent(event);
        break;
    }
}

VM::Window *Engine::window(void)
{
	return &m_window;
}

VM::Sound *Engine::sound(void)
{
	return &m_sound;
}

VOLCANO_SYSTEM_END
