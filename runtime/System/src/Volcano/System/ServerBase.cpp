//
//
#include <Volcano/System/ServerBase.h>

VOLCANO_SYSTEM_BEGIN

ServerBase::ServerBase()
    : host_(nullptr) {
}

ServerBase::~ServerBase() {
    if (host_ != nullptr) {
        enet_host_destroy(host_);
    }
}

bool ServerBase::start(const ENetAddress* address) {
    VOLCANO_ASSERT(host_ == nullptr);
    host_ = createHost(address);
    if (host_ == nullptr) {
        return false;
    }
    return true;
}

void ServerBase::update(Duration elapsed) {
    if (host_ != nullptr) {
        pollHost();
    }

    frame(elapsed);
}

void ServerBase::pollHost() {
    VOLCANO_ASSERT(host_ != nullptr);

    ENetEvent evt;
    int ret = enet_host_service(host_, &evt, 0);
    if (ret < 0) {
        return;
    }

    switch (evt.type) {
    case ENET_EVENT_TYPE_RECEIVE:
        handlePackage(*evt.packet);
        break;
    case ENET_EVENT_TYPE_CONNECT:
        handleConnect(*evt.peer);
        break;
    case ENET_EVENT_TYPE_DISCONNECT:
        handleDisconnect(*evt.peer);
        break;
    default:
        break;
    }
}

VOLCANO_SYSTEM_END
