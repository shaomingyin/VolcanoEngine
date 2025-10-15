//
//
#ifndef VOLCANO_FRAMEWORK_LOCAL_H
#define VOLCANO_FRAMEWORK_LOCAL_H

#include <string>

#include <SFML/Window/Event.hpp>

#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Gui/Context.h>

#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/Base.h>
#include <Volcano/Framework/Window.h>

VOLCANO_FRAMEWORK_BEGIN

class Local: public Base {
public:
    Local();
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
    Graphics::Renderer renderer_;
    Gui::Context hud_;
    Gui::Context* current_gui_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_LOCAL_H
