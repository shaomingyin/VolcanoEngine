//
//
#include <Volcano/Framework/Local.h>

VOLCANO_FRAMEWORK_BEGIN

Local::Local(const std::string& name)
    : Base(name)
    , window_({ 800, 600 }, name)
    , hud_(window_.getSize().x, window_.getSize().y)
    , renderer_(window_.getSize().x, window_.getSize().y) {
    window_.setFramerateLimit(60);
}

void Local::frame(Clock::duration elapsed) {
    sf::Event event;
    while (window_.pollEvent(event)) {
        handleEvent(event);
    }

    Base::frame(elapsed);

    window_.makeCurrent();
    renderer_.beginFrame();
    renderer_.endFrame();
    window_.display();
}

void Local::handleEvent(const sf::Event& event) {
    switch (event.type) {
    case sf::Event::MouseMoved:
        onMouseMoved(event.mouseMove);
        break;
    case sf::Event::MouseButtonPressed:
        onMouseButtonPressed(event.mouseButton);
        break;
    case sf::Event::MouseButtonReleased:
        onMouseButtonReleased(event.mouseButton);
        break;
    case sf::Event::KeyPressed:
        onKeyPressed(event.key);
        break;
    case sf::Event::KeyReleased:
        onKeyReleased(event.key);
        break;
    case sf::Event::MouseWheelMoved:
        break;
    case sf::Event::MouseWheelScrolled:
        break;
    case sf::Event::MouseEntered:
        break;
    case sf::Event::MouseLeft:
        break;
    case sf::Event::GainedFocus:
        break;
    case sf::Event::LostFocus:
        break;
    case sf::Event::Resized:
        break;
    case sf::Event::Closed:
        window_.close();
        quit();
        break;
    default:
        break;
    }
}

void Local::onMouseMoved(const sf::Event::MouseMoveEvent& event) {
}

void Local::onMouseButtonPressed(const sf::Event::MouseButtonEvent& event) {
}

void Local::onMouseButtonReleased(const sf::Event::MouseButtonEvent& event) {
}

void Local::onKeyPressed(const sf::Event::KeyEvent& event) {
}

void Local::onKeyReleased(const sf::Event::KeyEvent& event) {
}

VOLCANO_FRAMEWORK_END
