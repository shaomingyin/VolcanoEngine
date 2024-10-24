//
//
#include <Volcano/ScopeGuard.h>
#include <Volcano/Net/Client.h>

VOLCANO_NET_BEGIN

void Client::frame(Duration elapsed) {

}

void Client::handlePackage(const ENetPacket& package) {

}

void Client::handleConnect(ENetPeer& peer) {

}

void Client::handleDisconnect(ENetPeer& peer) {

}

ENetHost* Client::createHost(const ENetAddress* address) {
    if (address == nullptr) {
        return nullptr;
    }

    auto host = enet_host_create(nullptr, 1, 2, 0, 0);
    if (host == nullptr) {
        return nullptr;
    }

    auto host_guard = scopeGuard([host] {
        enet_host_destroy(host);
    });

    peer_ = enet_host_connect(host, address, 2, 0);
    if (peer_ == nullptr) {
        return nullptr;
    }

    host_guard.dismiss();

    return host;
}

VOLCANO_NET_END
