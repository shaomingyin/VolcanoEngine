//
//
#ifndef VOLCANO_SYSTEM_SERVER_H
#define VOLCANO_SYSTEM_SERVER_H

#include <string>

#include <Volcano/System/Common.h>
#include <Volcano/System/Base.h>

VOLCANO_SYSTEM_BEGIN

class Server: public Base {
public:
	Server(const std::string& manifest_path, int max_clients = 64);
	Server(const std::string& manifest_path, const ENetAddress& bind_address, int max_clients = 64);
	Server(const std::string& manifest_path, const std::string& host, int port = VOLCANO_SYSTEM_DEFAULT_PORT, int max_clients = 64);
	virtual ~Server() = default;

protected:
	void loadingFrame(Duration elapsed) override;
	void readyFrame(Duration elapsed) override;
	void playingFrame(Duration elapsed) override;
	void pausedFrame(Duration elapsed) override;
	void errorFrame(Duration elapsed) override;
	virtual void handlePackage(ENetPeer* peer, ENetPacket* package);
	virtual void handleConnect(ENetPeer* peer);
	virtual void handleDisconnect(ENetPeer* peer);

private:
	void init();
	void poll();

private:
	ENetAddress enet_bind_address_;
	ENetHost* enet_host_;
	int max_clients_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_SERVER_H
