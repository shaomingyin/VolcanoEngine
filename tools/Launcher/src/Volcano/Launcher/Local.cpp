//
//
#include <cassert>
#include <thread>

#include <Volcano/Launcher/Local.h>

VOLCANO_LAUNCHER_BEGIN

Local::Local(rttr::type scene_type)
    : state_(State::Idle)
    , scene_type_(scene_type)
    , scene_(nullptr)
    , frame_last_(Clock::now())
    , frame_count_last_(frame_last_)
    , frame_count_(0)
    , frame_count_per_second_(0)
    , window_(sf::VideoMode::getDesktopMode(), "VolcanoLauncher")
    , console_(nullptr)
    /*, renderer_(window_.getSize().x, window_.getSize().y) */{
    assert(scene_type_);
    assert(scene_type_.is_base_of(rttr::type::get<World::Scene>()));
    window_.setFramerateLimit(60);
}

void Local::run() {
    scene_instance_ = scene_type_.create({ *this });
    if (!scene_instance_) {
        throw std::runtime_error("Failed to create world scene instance.");
    }
    scene_ = &scene_instance_.get_value<World::Scene>();

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
        //scheduler_.run_all_tasks();
        auto now = Clock::now();
        auto elapsed = now - frame_last_;
        if (elapsed >= elapsed_min_) {
            //scene_->stepSimulation(elapsed);
            frame(elapsed);
            frame_last_ = now;
        } else {
            std::this_thread::sleep_for(elapsed_min_ - elapsed);
        }
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
    //renderer_.beginFrame();
    //renderer_.endFrame();
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
