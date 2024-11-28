//
//
#include <Volcano/Network/Server.h>

VOLCANO_NETWORK_BEGIN

Server::Server(int max_clients)
	: max_clients_(max_clients) {
	enet_bind_address_.host = ENET_HOST_ANY;
	enet_bind_address_.port = VOLCANO_NETWORK_DEFAULT_PORT;
	init();
}

Server::Server(const ENetAddress& bind_address, int max_clients)
	: enet_bind_address_(bind_address)
	, max_clients_(max_clients) {
	init();
}

Server::Server(const std::string& host, int port, int max_clients)
	: max_clients_(max_clients) {
	if (host != "any") {
		enet_address_set_host(&enet_bind_address_, host.c_str());
	} else {
		enet_bind_address_.host = ENET_HOST_ANY;
	}
	enet_bind_address_.port = port;
	init();
}

void Server::update(Duration elapsed) {
	Scene::update(elapsed);

	ENetEvent evt;
	int ret = enet_host_service(enet_host_, &evt, 0);
	if (ret == 0) {
		switch (evt.type) {
		case ENET_EVENT_TYPE_RECEIVE:
			handlePackage(evt.peer, evt.packet);
			break;
		case ENET_EVENT_TYPE_CONNECT:
			handleConnect(evt.peer);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			handleDisconnect(evt.peer);
			break;
		}
	}
}

void Server::handlePackage(ENetPeer* peer, ENetPacket* package) {
}

void Server::handleConnect(ENetPeer* peer) {
}

void Server::handleDisconnect(ENetPeer* peer) {
}

void Server::init() {
	enet_host_ = enet_host_create(&enet_bind_address_, max_clients_, 1, 0, 0);
	if (enet_host_ == nullptr) {
		throw std::runtime_error("Failed to create ENet server host.");
	}
}

VOLCANO_NETWORK_END
