//
//
#ifndef VOLCANO_NET_SERVER_HPP
#define VOLCANO_NET_SERVER_HPP

#include <QList>
#include <QUrl>
#include <QTimerEvent>
#include <QElapsedTimer>
#include <QSocketNotifier>
#include <QSocketDescriptor>
#include <QQmlListProperty>

#include <Volcano/Net/Common.hpp>
#include <Volcano/Net/Session.hpp>

VOLCANO_NET_BEGIN

#if 0
using SessionList = QList<Session *>;

class Server: public QObject {
    Q_OBJECT
    Q_PROPERTY(int tps READ tps NOTIFY tpsChanged)
    Q_PROPERTY(int tpsMax READ tpsMax WRITE setTpsMax NOTIFY tpsMaxChanged)
    Q_PROPERTY(bool running READ isRunning WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(int maxSession READ maxSession WRITE setMaxSession NOTIFY maxSessionChanged)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(Game::ActorSet *gameActorSet READ gameActorSet WRITE setGameActorSet NOTIFY gameActorSetChanged)
    Q_PROPERTY(QQmlListProperty<Session> sessions READ qmlSessions)

public:
    Server(QObject *parent = nullptr);
    ~Server(void) override;

public:
    int tps(void) const;
    int tpsMax(void) const;
    void setTpsMax(int v);
    bool isRunning(void) const;
    void setRunning(bool v);
    int maxSession(void) const;
    void setMaxSession(int v);
    const QUrl &url(void) const;
    void setUrl(const QUrl &v);
    Q_INVOKABLE bool start(void);
    Q_INVOKABLE void stop(void);
    Game::ActorSet *gameActorSet(void);
    void setGameActorSet(Game::ActorSet *p);
    const SessionList &sessions(void) const;
    QQmlListProperty<Session> qmlSessions(void);
    qsizetype sessionCount(void) const;
    Session *sessionAt(qsizetype index);

signals:
    void tpsChanged(int v);
    void tpsMaxChanged(int v);
    void runningChanged(bool v);
    void maxSessionChanged(int v);
    void urlChanged(const QUrl &v);
    void gameActorSetChanged(Game::ActorSet *p);

protected:
    void timerEvent(QTimerEvent *evt) override;
    void tick(Duration elapsed);

private:
    static qsizetype qmlSessionCount(QQmlListProperty<Session> *list);
    static Session *qmlSessionAt(QQmlListProperty<Session> *list, qsizetype index);
    void tryToRestart(void);
    void onPeerConnected(ENetPeer *enetPeer);
    void onPeerDisconnected(ENetPeer *enetPeer);
    void onPeerReceived(ENetPeer *enetPeer, const void *p, quint64 len);
    void pollENet(void);
    SessionList::iterator findSesssion(ENetPeer *enetPeer);

private:
    int m_tickTimer;
    int m_tickElapsedMin;
    int m_tickCount;
    int m_tickCountTimer;
    int m_tickCountPerSecond;
    QElapsedTimer m_tickElapsedTimer;
    int m_maxSession;
    QUrl m_url;
    Game::ActorSet *m_gameActorSet;
    QSocketNotifier m_rxNotifier;
    ENetHost *m_enetHost;
    QDataStream m_txStream;
    QByteArray m_txBuffer;
    SessionList m_sessions;
};
#endif

VOLCANO_NET_END

#endif // VOLCANO_NET_SERVER_HPP
