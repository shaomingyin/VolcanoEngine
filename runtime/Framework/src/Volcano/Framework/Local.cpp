//
//
#include <Volcano/Framework/Game.h>
#include <Volcano/Framework/Local.h>

VOLCANO_FRAMEWORK_BEGIN

Local::Local(SDL_Storage* rootfs, Game& game)
    : Base(rootfs, game)
    , window_("Volcano", 800, 600)
    , acoustics_space_(nullptr)
    , graphics_renderer_(nullptr) {
    initAcoustics();
    initGraphics();
    initGui();
}

void Local::event(const SDL_Event& evt) {
    window_.event(evt);
}

void Local::frame(Duration elapsed) {
    Base::frame(elapsed);

    if (acoustics_space_ != nullptr) {
        //acoustics_space_->
    }

}

void Local::loadConfig(const nlohmann::json& j) {
}

void Local::loadScene(const nlohmann::json& j) {
}

void Local::initAcoustics() {
}

void Local::initGraphics() {
}

void Local::initGui() {
}

VOLCANO_FRAMEWORK_END
