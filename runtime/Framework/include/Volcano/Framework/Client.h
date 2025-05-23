/*
 *
 */
#ifndef VOLCANO_FRAMEWORK_CLIENT_H
#define VOLCANO_FRAMEWORK_CLIENT_H

#include <ada.h>

#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/Local.h>

VOLCANO_FRAMEWORK_BEGIN

class Client: public Local {
public:
    Client(SDL_Storage* rootfs, SDL_Storage* userfs, rttr::type game_type, ada::url url);

public:
    void event(const SDL_Event& evt) override;
    void frame(Duration elapsed) override;

private:
    ada::url url_;
};

VOLCANO_FRAMEWORK_END

#endif /* VOLCANO_FRAMEWORK_CLIENT_H */
