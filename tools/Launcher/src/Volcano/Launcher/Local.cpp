//
//
#include <cassert>
#include <thread>

#include <Volcano/Launcher/Local.h>

VOLCANO_LAUNCHER_BEGIN

Local::Local(World::Scene& scene)
    : scene_(scene)
    , frame_last_(Clock::now())
    , frame_count_last_(frame_last_)
    , frame_count_(0)
    , frame_count_per_second_(0)
    , window_(sf::VideoMode::getDesktopMode(), "VolcanoLauncher")
    , renderer_(scene_)
    , console_(nullptr) {
    window_.setFramerateLimit(60);
}

void Local::schedule(async::task_run_handle task) {
    scheduler_.schedule(std::move(task));
}

void Local::run() {
    // TODO start loading

    frame_last_ = Clock::now();
    frame_count_last_ = frame_last_;
    frame_count_ = 0;
    frame_count_per_second_ = 0;

    while (window_.isOpen()) {
        while (true) {
            auto event = window_.pollEvent();
            if (event) {
                handleEvent(*event);
            } else {
                break;
            }
        }
        scheduler_.run_all_tasks();
        auto now = Clock::now();
        auto elapsed = now - frame_last_;
        frame(elapsed);
        if ((now - frame_count_last_) >= std::chrono::seconds(1)) {
            frame_count_per_second_ = frame_count_;
            frame_count_ = 0;
            frame_count_last_ = now;
        } else {
            frame_count_ += 1;
        }
        frame_last_ = now;
    }
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
    scene_.update(elapsed);
    renderer_.build(scene_.mainCamera());
    renderer_.draw(window_);
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

    if (auto p = event.getIf<sf::Event::MouseMoved>()) {
        onMouseMoved(*p);
    } else if (auto p = event.getIf<sf::Event::MouseButtonPressed>()) {
        onMouseButtonPressed(*p);
    } else if (auto p = event.getIf<sf::Event::MouseButtonReleased>()) {
        onMouseButtonReleased(*p);
    } else if (auto p = event.getIf<sf::Event::KeyPressed>()) {
        onKeyPressed(*p);
    } else if (auto p = event.getIf<sf::Event::KeyReleased>()) {
        onKeyReleased(*p);
    } else if (auto p = event.getIf<sf::Event::MouseWheelScrolled>()) {
    } else if (auto p = event.getIf<sf::Event::MouseEntered>()) {
    } else if (auto p = event.getIf<sf::Event::MouseLeft>()) {
    } else if (auto p = event.getIf<sf::Event::FocusGained>()) {
    } else if (auto p = event.getIf<sf::Event::FocusLost>()) {
    } else if (auto p = event.getIf<sf::Event::Resized>()) {
        onResized(*p);
    } else if (auto p = event.getIf<sf::Event::Closed>()) {
        window_.close();
    } else {
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
