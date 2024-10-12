//
//
#ifndef VOLCANO_SYSTEM_SERVER_H
#define VOLCANO_SYSTEM_SERVER_H

#include <Volcano/Game/World.h>
#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Server: public Game::World {
public:
    Server() = default;
    virtual ~Server();

public:

private:
    ENetHost enet_host_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_SERVER_H
