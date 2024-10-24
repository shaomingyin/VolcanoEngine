//
//
#ifndef VOLCANO_NET_CLIENT_H
#define VOLCANO_NET_CLIENT_H

#include <Volcano/Net/Common.h>
#include <Volcano/Net/Base.h>

VOLCANO_NET_BEGIN

class Client: public Base {
public:
    Client();
    ~Client() override;

protected:
    void frame(Duration elapsed) override;
    void handlePackage(const ENetPacket& package) override;
    void handleConnect(ENetPeer& peer) override;
    void handleDisconnect(ENetPeer& peer) override;
    ENetHost* createHost(const ENetAddress* address) override;

private:
    ENetPeer* peer_;
};

VOLCANO_NET_END

#endif // VOLCANO_NET_CLIENT_H
