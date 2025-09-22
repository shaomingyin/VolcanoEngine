//
//
#ifndef VOLCANO_NET_CLIENT_H
#define VOLCANO_NET_CLIENT_H

#include <string>

#include <Volcano/Net/Common.h>
#include <Volcano/Net/Local.h>

VOLCANO_NET_BEGIN

class Client: public Local {
public:
    Client(const ENetAddress& address);
    Client(const std::string& host, int port = VOLCANO_NET_DEFAULT_PORT);
	virtual ~Client();

protected:
    void tick(Duration elapsed) override;
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

VOLCANO_NET_END

#endif // VOLCANO_NET_CLIENT_H
