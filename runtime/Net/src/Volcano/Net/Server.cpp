//
//
#include <Volcano/Net/Server.h>

VOLCANO_NET_BEGIN

Server::Server(int max_clients)
    : max_clients_(max_clients) {
}

Server::~Server() {
}

bool Server::init() {
    return true;
}

void Server::frame(Duration elapsed) {
    Base::frame(elapsed);
}

void Server::handlePackage(const ENetPacket& package) {
}

void Server::handleConnect(ENetPeer& peer) {
}

void Server::handleDisconnect(ENetPeer& peer) {
}

ENetHost* Server::createHost(const ENetAddress* address) {
    if (address != nullptr) {
        return enet_host_create(address, max_clients_, 2, 0, 0);
    }

    ENetAddress bind_address = { ENET_HOST_ANY, VOLCANO_NET_DEFAULT_PORT };

    return enet_host_create(&bind_address, max_clients_, 2, 0, 0);
}

VOLCANO_NET_END
