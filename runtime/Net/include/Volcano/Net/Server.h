//
//
#ifndef VOLCANO_NET_SERVER_H
#define VOLCANO_NET_SERVER_H

#include <string>

#include <Volcano/Net/Common.h>
#include <Volcano/Net/Base.h>

VOLCANO_NET_BEGIN

class Server: public Base {
public:
	Server(int max_clients = 64);
	Server(const ENetAddress& bind_address, int max_clients = 64);
	Server(const std::string& host, int port = VOLCANO_NET_DEFAULT_PORT, int max_clients = 64);
	virtual ~Server() = default;

protected:
	void tick(Duration elapsed) override;
	virtual void handlePackage(ENetPeer* peer, ENetPacket* package);
	virtual void handleConnect(ENetPeer* peer);
	virtual void handleDisconnect(ENetPeer* peer);

private:
	void init();

private:
	ENetAddress enet_bind_address_;
	ENetHost* enet_host_;
	int max_clients_;
};

VOLCANO_NET_END

#endif // VOLCANO_NET_SERVER_H
