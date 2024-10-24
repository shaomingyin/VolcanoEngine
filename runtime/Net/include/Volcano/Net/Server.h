//
//
#ifndef VOLCANO_NET_SERVER_H
#define VOLCANO_NET_SERVER_H

#include <memory>

#include <Volcano/Net/Common.h>
#include <Volcano/Net/Base.h>

VOLCANO_NET_BEGIN

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
};

VOLCANO_NET_END

#endif // VOLCANO_NET_SERVER_H
