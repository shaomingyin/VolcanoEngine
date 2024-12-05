//
//
#include <Volcano/ScopeGuard.h>
#include <Volcano/System/Client.h>

VOLCANO_SYSTEM_BEGIN

Client::Client(const std::string& manifest_path, const ENetAddress& address)
	: Local(manifest_path)
	, enet_address_(address) {
}

Client::Client(const std::string& manifest_path, const std::string& host, int port)
	: Local(manifest_path) {
	enet_address_set_host(&enet_address_, host.c_str());
	enet_address_.port = port;
}

Client::~Client() {
	if (enet_peer_ != nullptr) {
		enet_peer_disconnect(enet_peer_, 0);
	}
	if (enet_host_ != nullptr) {
		enet_host_flush(enet_host_);
		enet_host_destroy(enet_host_);
	}
}

void Client::loadingFrame(Duration elapsed) {
	poll();
	Local::loadingFrame(elapsed);
}

void Client::readyFrame(Duration elapsed) {
	poll();
	Local::readyFrame(elapsed);
}

void Client::playingFrame(Duration elapsed) {
	poll();
	Local::playingFrame(elapsed);
}

void Client::pausedFrame(Duration elapsed) {
	poll();
	Local::pausedFrame(elapsed);
}

void Client::errorFrame(Duration elapsed) {
	poll();
	Local::loadingFrame(elapsed);
}

void Client::handlePackage(ENetPeer* peer, ENetPacket* package) {
}

void Client::handleConnect(ENetPeer* peer) {
}

void Client::handleDisconnect(ENetPeer* peer) {
}

void Client::init() {
	enet_host_ = enet_host_create(nullptr, 1, 0, 0, 0);
	if (enet_host_ == nullptr) {
		throw std::runtime_error("Failed to create ENet client host.");
	}

	auto enet_host_guard = scopeGuard([this] {
		enet_host_destroy(enet_host_);
		enet_host_ = nullptr;
		});

	enet_peer_ = enet_host_connect(enet_host_, &enet_address_, sizeof(ENetAddress), 0);
	if (enet_peer_ == nullptr) {
		throw std::runtime_error("Failed to create ENet connection.");
	}

	enet_host_guard.dismiss();
}

void Client::poll() {
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

VOLCANO_SYSTEM_END
