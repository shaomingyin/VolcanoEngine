//
//
#include <Volcano/ScopeGuard.h>
#include <Volcano/System/ServerProxy.h>

VOLCANO_SYSTEM_BEGIN

void ServerProxy::frame(Duration elapsed) {

}

void ServerProxy::handlePackage(const ENetPacket& package) {

}

void ServerProxy::handleConnect(ENetPeer& peer) {

}

void ServerProxy::handleDisconnect(ENetPeer& peer) {

}

ENetHost* ServerProxy::createHost(const ENetAddress* address) {
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

    return host;
}

VOLCANO_SYSTEM_END
