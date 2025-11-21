//
//
#ifndef VOLCANO_LAUNCHER_WINDOW_HPP
#define VOLCANO_LAUNCHER_WINDOW_HPP

#include <GL/gl3w.h>
#include <SFML/Graphics/RenderWindow.hpp>

#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

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

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_WINDOW_HPP
