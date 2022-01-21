//
//
#ifndef VOLCANO_NET_SESSION_HPP
#define VOLCANO_NET_SESSION_HPP

#include <QByteArray>
#include <QObject>

#include <Volcano/Net/Common.hpp>

VOLCANO_NET_BEGIN

class Session: public QObject {
    Q_OBJECT

public:
    Session(ENetPeer *enetPeer, QObject *parent = nullptr);
    ~Session(void) override;

public:
    ENetPeer *toENetPeer(void);
    static void handleReceived(ENetPeer *enetPeer, const QByteArray &data);

private:
    void onReceived(const QByteArray &data);

private:
    ENetPeer *m_enetPeer;
};

VOLCANO_NET_END

#endif // VOLCANO_NET_SESSION_HPP
