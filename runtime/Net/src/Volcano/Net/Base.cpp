//
//
#include <Volcano/Net/Base.h>

VOLCANO_NET_BEGIN

Base::Base()
    : host_(nullptr) {
}

Base::~Base() {
    if (host_ != nullptr) {
        enet_host_destroy(host_);
    }
}

bool Base::start(const ENetAddress* address) {
    VOLCANO_ASSERT(host_ == nullptr);
    host_ = createHost(address);
    if (host_ == nullptr) {
        return false;
    }
    return true;
}

void Base::frame(Duration elapsed) {
    if (host_ != nullptr) {
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

    Game::World::frame(elapsed);
}

VOLCANO_NET_END
