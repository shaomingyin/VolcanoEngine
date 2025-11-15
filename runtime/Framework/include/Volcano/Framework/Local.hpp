//
//
#ifndef VOLCANO_FRAMEWORK_LOCAL_HPP
#define VOLCANO_FRAMEWORK_LOCAL_HPP

#include <string>

#include <SFML/Window/Event.hpp>

#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Gui/Context.hpp>

#include <Volcano/Framework/Common.hpp>
#include <Volcano/Framework/Base.hpp>
#include <Volcano/Framework/Window.hpp>
#include <Volcano/Framework/Console.hpp>

VOLCANO_FRAMEWORK_BEGIN

class Local: public Base {
public:
    Local(WorldCreator world_creator);
    virtual ~Local() = default;

protected:
    void frame(Clock::duration elapsed) noexcept override;
    virtual void handleEvent(const sf::Event& event);
    virtual void onMouseMoved(const sf::Event::MouseMoveEvent& event);
    virtual void onMouseButtonPressed(const sf::Event::MouseButtonEvent& event);
    virtual void onMouseButtonReleased(const sf::Event::MouseButtonEvent& event);
    virtual void onKeyPressed(const sf::Event::KeyEvent& event);
    virtual void onKeyReleased(const sf::Event::KeyEvent& event);
    virtual void onResized(const sf::Event::SizeEvent& event);

private:
    Window window_;
    //Gui::Context window_gui_context_;
    Console* console_;
    Graphics::Renderer renderer_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_LOCAL_HPP
