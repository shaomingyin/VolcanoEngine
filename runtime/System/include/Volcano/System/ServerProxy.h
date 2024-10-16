//
//
#ifndef VOLCANO_SYSTEM_SERVERPROXY_H
#define VOLCANO_SYSTEM_SERVERPROXY_H

#include <Volcano/System/Common.h>
#include <Volcano/System/ServerBase.h>

VOLCANO_SYSTEM_BEGIN

class ServerProxy: public ServerBase {
public:
    ServerProxy();
    ~ServerProxy() override;

protected:
    void frame(Duration elapsed) override;
    void handlePackage(const ENetPacket& package) override;
    void handleConnect(ENetPeer& peer) override;
    void handleDisconnect(ENetPeer& peer) override;
    ENetHost* createHost(const ENetAddress* address) override;

private:
    ENetPeer* peer_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_SERVERPROXY_H
