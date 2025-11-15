//
//
#include <Volcano/Framework/Local.hpp>

VOLCANO_FRAMEWORK_BEGIN

Local::Local(WorldCreator world_creator)
    : Base(std::move(world_creator))
    , window_({ 800, 600 }, "VolcanoLauncher")
    , console_(nullptr)
    , renderer_(window_.getSize().x, window_.getSize().y) {
    window_.setFramerateLimit(60);
}

void Local::frame(Clock::duration elapsed) noexcept {
    if (!window_.isOpen()) {
        window_.close();
        return;
    }

    sf::Event event;
    while (window_.pollEvent(event)) {
        handleEvent(event);
    }

    Base::frame(elapsed);

    renderer_.beginFrame();
    renderer_.endFrame();
    //hud_.render();
    window_.display();
}

void Local::handleEvent(const sf::Event& event) {
    // if (event.type == sf::Event::KeyPressed && event.key.code == sf::Event::KeyEvent::code) {
    //     console_.taggle();
    // }

    // if (current_gui_ != nullptr) {
    //     current_gui_->handleEvent(event);
    //     return;
    // }

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
        onResized(event.size);
        break;
    case sf::Event::Closed:
        window_.close();
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

void Local::onResized(const sf::Event::SizeEvent& event) {
    //hud_.resize(event.width, event.height);
    renderer_.resize(event.width, event.height);
}

VOLCANO_FRAMEWORK_END
