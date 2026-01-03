//
//
#include <cassert>

#include <Volcano/Launcher/Local.hpp>

VOLCANO_LAUNCHER_BEGIN

Local::Local(WorldCreator world_creator)
    : world_creator_(std::move(world_creator))
    , frame_last_(Clock::now())
    , frame_count_last_(frame_last_)
    , frame_count_(0)
    , frame_count_per_second_(0)
    , window_(sf::VideoMode::getDesktopMode(), "VolcanoLauncher")
    , console_(nullptr)
    /*, renderer_(window_.getSize().x, window_.getSize().y) */{
    assert(world_creator_);
    window_.setFramerateLimit(60);
}

void Local::run() {

}

void Local::schedule(async::task_run_handle t) {
    scheduler_.schedule(std::move(t));
}

unsigned long Local::fps() const noexcept {
    return 0;
}

unsigned long Local::fpsMax() const noexcept {
    return 0;
}

void Local::setFpsMax(unsigned long v) noexcept {
}

void Local::frame(Clock::duration elapsed) noexcept {
    scheduler_.run_all_tasks();

    if (!window_.isOpen()) {
        window_.close();
        return;
    }

    std::optional<sf::Event> evt;
    while (evt = window_.pollEvent()) {
        handleEvent(*evt);
    }

    //renderer_.beginFrame();
    //renderer_.endFrame();
    //hud_.render();
    window_.display();
}

void Local::handleEvent(const sf::Event& evt) {
    // if (event.type == sf::Event::KeyPressed && event.key.code == sf::Event::KeyEvent::code) {
    //     console_.taggle();
    // }

    // if (current_gui_ != nullptr) {
    //     current_gui_->handleEvent(event);
    //     return;
    // }

    auto mouse_moved = evt.getIf<sf::Event::MouseMoved>();
    if (mouse_moved != nullptr) {
        onMouseMoved(*mouse_moved);
        return;
    }

    auto mouse_button_pressed = evt.getIf<sf::Event::MouseButtonPressed>();
    if (mouse_button_pressed != nullptr) {
        onMouseButtonPressed(*mouse_button_pressed);
        return;
    }

    auto mouse_button_released = evt.getIf<sf::Event::MouseButtonReleased>();
    if (mouse_button_released != nullptr) {
        onMouseButtonReleased(*mouse_button_released);
        return;
    }

    auto key_pressed = evt.getIf<sf::Event::KeyPressed>();
    if (key_pressed != nullptr) {
        onKeyPressed(*key_pressed);
        return;
    }

    auto key_released = evt.getIf<sf::Event::KeyReleased>();
    if (key_released != nullptr) {
        onKeyReleased(*key_released);
        return;
    }
}

void Local::onMouseMoved(const sf::Event::MouseMoved& event) {
}

void Local::onMouseButtonPressed(const sf::Event::MouseButtonPressed& event) {
}

void Local::onMouseButtonReleased(const sf::Event::MouseButtonReleased& event) {
}

void Local::onKeyPressed(const sf::Event::KeyPressed& event) {
}

void Local::onKeyReleased(const sf::Event::KeyReleased& event) {
}

void Local::onResized(const sf::Event::Resized& event) {
    //hud_.resize(event.width, event.height);
    //renderer_.resize(event.width, event.height);
}

VOLCANO_LAUNCHER_END
