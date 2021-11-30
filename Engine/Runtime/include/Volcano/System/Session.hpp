//
//
#ifndef VOLCANO_SYSTEM_SESSION_HPP
#define VOLCANO_SYSTEM_SESSION_HPP

#include <QByteArray>
#include <QObject>

#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

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

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_SESSION_HPP
