//
//
#include <Volcano/Error.h>
#include <Volcano/System/Server.h>

VOLCANO_SYSTEM_BEGIN

Server::Server(const std::filesystem::path& root, const std::filesystem::path& init, int max_clients)
	: Base(root, init, 0)
	, max_clients_(max_clients) {
	enet_bind_address_.host = ENET_HOST_ANY;
	enet_bind_address_.port = VOLCANO_SYSTEM_DEFAULT_PORT;
}

Server::Server(const std::filesystem::path& root, const std::filesystem::path& init, const ENetAddress& bind_address, int max_clients)
	: Base(root, init, 0)
	, enet_bind_address_(bind_address)
	, max_clients_(max_clients) {
}

Server::Server(const std::filesystem::path& root, const std::filesystem::path& init, const std::string& host, int port, int max_clients)
	: Base(root, init, 0)
	, max_clients_(max_clients) {
	if (host != "any") {
		enet_address_set_host(&enet_bind_address_, host.c_str());
	} else {
		enet_bind_address_.host = ENET_HOST_ANY;
	}
	enet_bind_address_.port = port;
}

void Server::init() {
	Base::init();
	enet_host_ = enet_host_create(&enet_bind_address_, max_clients_, 1, 0, 0);
	if (enet_host_ == nullptr) {
		throw Error(Errc::OutOfResource);
	}
}

void Server::frame(Duration elapsed) {
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

	Base::frame(elapsed);
}

void Server::handlePackage(ENetPeer* peer, ENetPacket* package) {
}

void Server::handleConnect(ENetPeer* peer) {
}

void Server::handleDisconnect(ENetPeer* peer) {
}

VOLCANO_SYSTEM_END
