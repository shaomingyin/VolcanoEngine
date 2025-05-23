//
//
#include <Volcano/Framework/Client.h>

VOLCANO_FRAMEWORK_BEGIN

Client::Client(SDL_Storage* rootfs, SDL_Storage* userfs, rttr::type game_type, ada::url url)
    : Local(rootfs, userfs, game_type)
    , url_(std::move(url)) {
}

void Client::event(const SDL_Event& evt) {
    Local::event(evt);
}

void Client::frame(Duration elapsed) {
    Local::frame(elapsed);
}

VOLCANO_FRAMEWORK_END
