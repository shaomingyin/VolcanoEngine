//
//
#include <Volcano/Framework/Client.h>

VOLCANO_FRAMEWORK_BEGIN

Client::Client(SDL_Storage* rootfs, ada::url url)
    : Local(rootfs)
    , url_(std::move(url)) {
}

void Client::event(const SDL_Event& evt) {
    Local::event(evt);
}

void Client::frame(Duration elapsed) {
    Local::frame(elapsed);
}

VOLCANO_FRAMEWORK_END
