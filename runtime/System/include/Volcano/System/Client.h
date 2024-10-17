//
//
#ifndef VOLCANO_SYSTEM_CLIENT_H
#define VOLCANO_SYSTEM_CLIENT_H

#include <Volcano/System/Common.h>
#include <Volcano/System/Base.h>

VOLCANO_SYSTEM_BEGIN

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

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_CLIENT_H
