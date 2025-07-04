//
//
#include <Volcano/Framework/Server.h>

VOLCANO_FRAMEWORK_BEGIN

Server::Server(SDL_Storage* rootfs, Game& game, std::vector<ada::url> urls)
    : Base(rootfs, game)
    , urls_(std::move(urls)) {
}

void Server::frame(Duration elapsed) {
    Base::frame(elapsed);
}

VOLCANO_FRAMEWORK_END
