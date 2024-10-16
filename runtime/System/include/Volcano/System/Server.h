//
//
#ifndef VOLCANO_SYSTEM_SERVER_H
#define VOLCANO_SYSTEM_SERVER_H

#include <Volcano/Game/World.h>
#include <Volcano/System/Common.h>
#include <Volcano/System/ServerBase.h>

VOLCANO_SYSTEM_BEGIN

class Server: public ServerBase {
public:
    Server(int max_clients = 64);
    virtual ~Server();

public:
    bool init();

protected:
    void frame(Duration elapsed) override;
    void handlePackage(const ENetPacket& package) override;
    void handleConnect(ENetPeer& peer) override;
    void handleDisconnect(ENetPeer& peer) override;
    ENetHost* createHost(const ENetAddress* bind_address) override;

private:
    Game::Context game_context_;
    int max_clients_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_SERVER_H
