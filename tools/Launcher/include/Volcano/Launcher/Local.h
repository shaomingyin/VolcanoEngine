//
//
#ifndef VOLCANO_LAUNCHER_LOCAL_H
#define VOLCANO_LAUNCHER_LOCAL_H

#include <functional>

#include <rttr/type>

#include <SFML/Window/Event.hpp>

#include <Volcano/Graphics/Context.h>
#include <Volcano/Gui/Context.h>

#include <Volcano/World/Scene.h>
#include <Volcano/Launcher/Console.h>
#include <Volcano/Launcher/Window.h>
#include <Volcano/Launcher/Common.h>

VOLCANO_LAUNCHER_BEGIN

class Local {
public:
    using Clock = World::Clock;

public:
    Local(rttr::type scene_type);
    virtual ~Local() = default;

public:
    void run();
    unsigned long fps() const noexcept;
    unsigned long fpsMax() const noexcept;
    void setFpsMax(unsigned long v) noexcept;

protected:
    enum State {
        Idle = 0,
        Loading,
        Ready,
        Playing,
        Paused,
        Error
    };

    virtual void frame(Clock::duration elapsed) noexcept;
    virtual void handleEvent(const sf::Event& event);
    virtual void onMouseMoved(const sf::Event::MouseMoved& event);
    virtual void onMouseButtonPressed(const sf::Event::MouseButtonPressed& event);
    virtual void onMouseButtonReleased(const sf::Event::MouseButtonReleased& event);
    virtual void onKeyPressed(const sf::Event::KeyPressed& event);
    virtual void onKeyReleased(const sf::Event::KeyReleased& event);
    virtual void onResized(const sf::Event::Resized& event);

private:

private:
    State state_;
    rttr::type scene_type_;
    rttr::variant scene_instance_;
    World::Scene* scene_;
    Clock::duration elapsed_min_;
    Clock::time_point frame_last_;
    Clock::time_point frame_count_last_;
    unsigned long frame_count_;
    unsigned long frame_count_per_second_;
    Window window_;
    Console* console_;
    //Graphics::Context graphics_context_;
    //Gui::Context gui_context_;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_LOCAL_H
