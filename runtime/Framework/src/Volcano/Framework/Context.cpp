//
//
#include <Volcano/Framework/Context.h>

VOLCANO_FRAMEWORK_BEGIN

Context::Context(SDL_Storage* rootfs, SDL_Storage* userfs)
    : state_(State::Idle)
    , quit_(false)
    , rootfs_(rootfs)
    , userfs_(userfs)
    , loading_total_(0)
    , loading_progress_(0) {
}

void Context::init() {
    try {
        loadConfig(nlohmann::loadFromStorage(rootfs_, "/Config.json"));
        //loadConfig(nlohmann::loadFromStorage(userfs_, "/Config.json"));
    } catch (...) {
    }
}

void Context::update(Duration elapsed) {
    scheduler_.run_all_tasks();
}

VOLCANO_FRAMEWORK_END
