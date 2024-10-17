//
//
#ifndef VOLCANO_SYSTEM_SERVER_H
#define VOLCANO_SYSTEM_SERVER_H

#include <memory>

#include <Volcano/Game/World.h>
#include <Volcano/System/Common.h>
#include <Volcano/System/Base.h>

VOLCANO_SYSTEM_BEGIN

class Server: public Base {
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
    int max_clients_;
    std::unique_ptr<Game::World> game_world_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_SERVER_H
