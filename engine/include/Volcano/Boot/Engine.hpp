//
//
#ifndef VOLCANO_BOOT_ENGINE_HPP
#define VOLCANO_BOOT_ENGINE_HPP

#include <Volcano/VM/Kernel.hpp>
#include <Volcano/Graphics/Window.hpp>
#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Boot/Common.hpp>

VOLCANO_BOOT_BEGIN

class Engine: public VM::Kernel {
public:
    Engine(uv_loop_t *loop);
    ~Engine(void) override;

protected:
    bool init(void) override;
    void shutdown(void) override;
    void initExports(VM::Registration &reg) override;
    void frame(float elapsed) override;
    void handleEvent(const SDL_Event &evt) override;

private:
    uv_async_t m_renderAsync;
    Graphics::Window m_window;
    Graphics::Renderer m_renderer;
    Graphics::View m_viewFlip[2];
    int m_viewRendering;
    SpinLock m_viewLock;
};

VOLCANO_BOOT_END

#endif // VOLCANO_BOOT_ENGINE_HPP
