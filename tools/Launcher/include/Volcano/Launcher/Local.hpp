//
//
#ifndef VOLCANO_LAUNCHER_LOCAL_HPP
#define VOLCANO_LAUNCHER_LOCAL_HPP

#include <functional>

#include <rttr/type>

#include <SFML/Window/Event.hpp>

#include <Volcano/Graphics/Context.hpp>
#include <Volcano/Gui/Context.hpp>

#include <Volcano/Game/Context.hpp>
#include <Volcano/Game/World.hpp>
#include <Volcano/Launcher/Console.hpp>
#include <Volcano/Launcher/Window.hpp>
#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

class Local: public Game::Context {
public:
    Local(rttr::type world_type);
    ~Local() override = default;

public:
    void run();
    void schedule(async::task_run_handle t) override;
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
    virtual void onMouseMoved(const sf::Event::MouseMoveEvent& event);
    virtual void onMouseButtonPressed(const sf::Event::MouseButtonEvent& event);
    virtual void onMouseButtonReleased(const sf::Event::MouseButtonEvent& event);
    virtual void onKeyPressed(const sf::Event::KeyEvent& event);
    virtual void onKeyReleased(const sf::Event::KeyEvent& event);
    virtual void onResized(const sf::Event::SizeEvent& event);

private:

private:
    State state_;
    rttr::type world_type_;
    rttr::variant world_instance_;
    Game::World* world_;
    async::fifo_scheduler scheduler_;
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

#endif // VOLCANO_LAUNCHER_LOCAL_HPP
