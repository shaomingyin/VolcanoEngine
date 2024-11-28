//
//
#ifndef VOLCANO_NETWORK_SERVER_H
#define VOLCANO_NETWORK_SERVER_H

#include <string>

#include <Volcano/World/Scene.h>
#include <Volcano/Network/Common.h>

VOLCANO_NETWORK_BEGIN

class Server: public World::Scene {
public:
	Server(int max_clients = 64);
	Server(const ENetAddress& bind_address, int max_clients = 64);
	Server(const std::string& host, int port = VOLCANO_NETWORK_DEFAULT_PORT, int max_clients = 64);
	virtual ~Server() = default;

protected:
	void update(Duration elapsed) override;
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

VOLCANO_NETWORK_END

#endif // VOLCANO_NETWORK_SERVER_H
