//
//
#ifndef VOLCANO_NET_BASE_H
#define VOLCANO_NET_BASE_H

#include <Volcano/Game/World.h>
#include <Volcano/Net/Common.h>

VOLCANO_NET_BEGIN

class Base: public Game::World {
public:
    Base();
    virtual ~Base();

public:
    bool start(const ENetAddress* address = nullptr);
    void stop();
    void update(Duration elapsed);

    bool isStarted() const {
        return (host_ != nullptr);
    }

    const ENetAddress& address() const {
        VOLCANO_ASSERT(host_ != nullptr);
        return host_->address;
    }

protected:
    virtual void frame(Duration elapsed) = 0;
    virtual void handlePackage(const ENetPacket& package) = 0;
    virtual void handleConnect(ENetPeer& peer) = 0;
    virtual void handleDisconnect(ENetPeer& peer) = 0;
    virtual ENetHost* createHost(const ENetAddress* address) = 0;

private:
    void pollHost();

private:
    ENetHost* host_;
};

VOLCANO_NET_END

#endif // VOLCANO_NET_BASE_H
