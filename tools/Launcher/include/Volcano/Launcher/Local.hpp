//
//
#ifndef VOLCANO_LAUNCHER_LOCAL_HPP
#define VOLCANO_LAUNCHER_LOCAL_HPP

#include <functional>

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
    using WorldCreator = std::function<Game::World* (Game::Context&)>;

public:
    Local(WorldCreator world_creator);
    ~Local() override = default;

public:
    void run();
    void schedule(async::task_run_handle t) override;
    unsigned long fps() const noexcept override;
    unsigned long fpsMax() const noexcept override;
    void setFpsMax(unsigned long v) noexcept override;

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
    WorldCreator world_creator_;
    async::fifo_scheduler scheduler_;
    Clock::duration min_elapsed_;
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
