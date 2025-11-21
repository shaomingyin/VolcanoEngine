//
//
#include <cassert>
#include <thread>

#include <Volcano/Launcher/Local.hpp>

VOLCANO_LAUNCHER_BEGIN

Local::Local(rttr::type scene_type)
    : state_(State::Idle)
    , scene_type_(std::move(scene_type))
    , scene_(nullptr)
    , frame_last_(Clock::now())
    , frame_count_last_(frame_last_)
    , frame_count_(0)
    , frame_count_per_second_(0)
    , window_({ 800, 600 }, "VolcanoLauncher")
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
        sf::Event event;
        while (window_.pollEvent(event)) {
            handleEvent(event);
        }
        scheduler_.run_all_tasks();
        auto now = Clock::now();
        auto elapsed = now - frame_last_;
        if (elapsed >= elapsed_min_) {
            scene_->stepSimulation(elapsed);
            frame(elapsed);
            frame_last_ = now;
        } else {
            std::this_thread::sleep_for(elapsed_min_ - elapsed);
        }
    }
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
    //renderer_.resize(event.width, event.height);
}

VOLCANO_LAUNCHER_END
