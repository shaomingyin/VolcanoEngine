//
//
#ifndef VOLCANO_NET_SESSION_HPP
#define VOLCANO_NET_SESSION_HPP

#include <QString>
#include <QObject>
#include <QByteArray>

#include <Volcano/Net/Common.hpp>

VOLCANO_NET_BEGIN

class Session: public QObject {
    Q_OBJECT
    Q_PROPERTY(QString host READ host NOTIFY hostChanged)
    Q_PROPERTY(quint16 port READ port NOTIFY portChanged)
    Q_PROPERTY(quint32 pingInterval READ pingInterval WRITE setPingInterval NOTIFY pingIntervalChanged)
    Q_PROPERTY(quint32 rtt READ rtt NOTIFY rttChanged)

public:
    Session(QObject *parent = nullptr);
    virtual ~Session(void);

public:
    bool isMatch(const ENetPeer *peer) const;
    const QString &host(void) const;
    quint16 port(void) const;
    quint32 pingInterval(void) const;
    void setPingInterval(quint32 v);
    quint32 rtt(void) const;
    Q_INVOKABLE void reset(void);
    Q_INVOKABLE void disconnect(void);
    bool send(const QByteArray &data);

signals:
    void hostChanged(const QString &v);
    void portChanged(quint16 port);
    void pingIntervalChanged(quint32 v);
    void rttChanged(quint32 v);
    void dataReceived(const QByteArray &data);

protected:
    bool init(ENetPeer *peer);
    virtual void onReceived(const QByteArray &data);
    static Session *fromENetPeer(ENetPeer *peer);

private:
    QString m_host;
    quint16 m_port;
    ENetPeer *m_enetPeer;

    friend class Server;
};

VOLCANO_NET_END

#endif // VOLCANO_NET_SESSION_HPP
