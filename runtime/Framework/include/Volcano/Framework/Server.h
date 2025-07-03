/*
 *
 */
#ifndef VOLCANO_FRAMEWORK_SERVER_H
#define VOLCANO_FRAMEWORK_SERVER_H

#include <vector>

#include <ada.h>

#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/Game.h>
#include <Volcano/Framework/Base.h>

VOLCANO_FRAMEWORK_BEGIN

class Server: public Base {
public:
    Server(SDL_Storage* rootfs, std::vector<ada::url> urls);

protected:
    virtual void frame(Duration elapsed);

private:
    std::vector<ada::url> urls_;
};

VOLCANO_FRAMEWORK_END

#endif /* VOLCANO_FRAMEWORK_SERVER_H */
