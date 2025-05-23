/*
 *
 */
#ifndef VOLCANO_FRAMEWORK_SERVER_H
#define VOLCANO_FRAMEWORK_SERVER_H

#include <vector>

#include <ada.h>

#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/Game.h>
#include <Volcano/Framework/ContextImpl.h>

VOLCANO_FRAMEWORK_BEGIN

class Server: public ContextImpl {
public:
    Server(SDL_Storage* rootfs, SDL_Storage* userfs, rttr::type game_type, std::vector<ada::url> urls);

public:
    virtual void frame(Duration elapsed);

private:
    std::vector<ada::url> urls_;
};

VOLCANO_FRAMEWORK_END

#endif /* VOLCANO_FRAMEWORK_SERVER_H */
