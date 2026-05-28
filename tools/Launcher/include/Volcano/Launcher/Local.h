//
//
#ifndef VOLCANO_LAUNCHER_LOCAL_H
#define VOLCANO_LAUNCHER_LOCAL_H

#include <async++.h>

#include <SFML/Window/Event.hpp>

#include <Volcano/Gui/Context.h>
#include <Volcano/World/Scene.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Launcher/Common.h>
#include <Volcano/Launcher/Console.h>
#include <Volcano/Launcher/Window.h>

VOLCANO_LAUNCHER_BEGIN

class Local {
public:
	using Clock = World::Clock;

public:
    Local(World::Scene& scene);
    virtual ~Local() = default;

public:
    void schedule(async::task_run_handle task);
    void run();
    unsigned long fps() const noexcept;
    unsigned long fpsMax() const noexcept;
    void setFpsMax(unsigned long v) noexcept;

protected:
    virtual void frame(Clock::duration elapsed) noexcept;
    virtual void handleEvent(const sf::Event& event);
    virtual void onMouseMoved(const sf::Event::MouseMoved& event);
    virtual void onMouseButtonPressed(const sf::Event::MouseButtonPressed& event);
    virtual void onMouseButtonReleased(const sf::Event::MouseButtonReleased& event);
    virtual void onKeyPressed(const sf::Event::KeyPressed& event);
    virtual void onKeyReleased(const sf::Event::KeyReleased& event);
    virtual void onResized(const sf::Event::Resized& event);

private:
	async::fifo_scheduler scheduler_;
	World::Scene& scene_;
    Clock::duration elapsed_min_;
    Clock::time_point frame_last_;
    Clock::time_point frame_count_last_;
    unsigned long frame_count_;
    unsigned long frame_count_per_second_;
    Window window_;
    Graphics::Renderer renderer_;
    Console* console_;
    //Graphics::Context graphics_context_;
    //Gui::Context gui_context_;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_LOCAL_H
