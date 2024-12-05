//
//
#ifndef VOLCANO_SYSTEM_CLIENT_H
#define VOLCANO_SYSTEM_CLIENT_H

#include <string>

#include <Volcano/System/Common.h>
#include <Volcano/System/Local.h>

VOLCANO_SYSTEM_BEGIN

class Client: public Local {
public:
    Client(const std::string& manifest_path, const ENetAddress& address);
    Client(const std::string& manifest_path, const std::string& host, int port = VOLCANO_SYSTEM_DEFAULT_PORT);
	virtual ~Client();

protected:
    void loadingFrame(Duration elapsed) override;
    void readyFrame(Duration elapsed) override;
    void playingFrame(Duration elapsed) override;
    void pausedFrame(Duration elapsed) override;
    void errorFrame(Duration elapsed) override;
    virtual void handlePackage(ENetPeer* peer, ENetPacket* package);
    virtual void handleConnect(ENetPeer* peer) ;
    virtual void handleDisconnect(ENetPeer* peer);

private:
    void init();
    void poll();

private:
    ENetAddress enet_address_;
    ENetHost* enet_host_;
    ENetPeer* enet_peer_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_CLIENT_H
