//
//
#include <Volcano/Framework/Server.h>

VOLCANO_FRAMEWORK_BEGIN

Server::Server(SDL_Storage* rootfs, SDL_Storage* userfs, rttr::type game_type, std::vector<ada::url> urls)
    : ContextImpl(rootfs, userfs, game_type)
    , urls_(std::move(urls)) {
}

void Server::frame(Duration elapsed) {
    ContextImpl::frame(elapsed);

    // TODO
}

VOLCANO_FRAMEWORK_END
