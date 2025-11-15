//
//
#ifndef VOLCANO_FRAMEWORK_WINDOW_HPP
#define VOLCANO_FRAMEWORK_WINDOW_HPP

#include <GL/gl3w.h>
#include <SFML/Graphics/RenderWindow.hpp>

#include <Volcano/Framework/Common.hpp>

VOLCANO_FRAMEWORK_BEGIN

class Window: public sf::RenderWindow {
public:
    Window(sf::VideoMode mode, const sf::String& title, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());

public:
    void makeCurrent() noexcept {
        gl3wProcs = &gl3w_;
    }

private:
    GL3WProcs gl3w_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_WINDOW_HPP
