//
//
#ifndef VOLCANO_SYSTEM_SINGLEPLAYER_H
#define VOLCANO_SYSTEM_SINGLEPLAYER_H

#include <Volcano/System/Common.h>
#include <Volcano/System/Client.h>
#include <Volcano/System/Server.h>

VOLCANO_SYSTEM_BEGIN

class SinglePlayer {
public:
    SinglePlayer();
    virtual ~SinglePlayer();

public:

private:
    Client client_;
    Server server_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_SINGLEPLAYER_H
