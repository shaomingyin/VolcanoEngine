//
//
#ifndef VOLCANO_NETWORK_CLIENT_H
#define VOLCANO_NETWORK_CLIENT_H

#include <string>

#include <Volcano/World/Scene.h>
#include <Volcano/Network/Common.h>

VOLCANO_NETWORK_BEGIN

class Client: public World::Scene {
public:
    Client(const ENetAddress& address);
    Client(const std::string& host, int port = VOLCANO_NETWORK_DEFAULT_PORT);
	virtual ~Client();

protected:
    void update(Duration elapsed) override;
    virtual void handlePackage(ENetPeer* peer, ENetPacket* package);
    virtual void handleConnect(ENetPeer* peer) ;
    virtual void handleDisconnect(ENetPeer* peer);

private:
    void init();

private:
    ENetAddress enet_address_;
    ENetHost* enet_host_;
    ENetPeer* enet_peer_;
};

VOLCANO_NETWORK_END

#endif // VOLCANO_NETWORK_CLIENT_H
