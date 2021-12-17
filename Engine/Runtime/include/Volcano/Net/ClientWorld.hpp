//
//
#ifndef VOLCANO_NET_CLIENTWORLD_HPP
#define VOLCANO_NET_CLIENTWORLD_HPP

#include <QUrl>
#include <QSocketNotifier>
#include <QSocketDescriptor>
#include <QQmlListProperty>

#include <Volcano/Game/WorldBase.hpp>
#include <Volcano/Net/Common.hpp>

VOLCANO_NET_BEGIN

class ClientWorld: public Game::WorldBase {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)

public:
    enum State {
        STATE_DISABLED = 0,
        STATE_ENABLING,
        STATE_ENABLED,
        STATE_DISABLING,
        STATE_ERROR
    };
    Q_ENUM(State)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)

public:
    ClientWorld(QObject *parent = nullptr);
    ~ClientWorld(void) override;

public:
    State state(void) const;
    bool isEnabled(void) const;
    void setEnabled(bool v);
    const QUrl &url(void) const;
    void setUrl(const QUrl &v);
    Q_INVOKABLE bool enable(void);
    Q_INVOKABLE void disable(void);

signals:
    void stateChanged(State v);
    void enabledChanged(bool v);
    void urlChanged(const QUrl &v);

protected:
    void setState(State v);

private:
    void onPeerConnected(ENetPeer *enetPeer);
    void onPeerDisconnected(ENetPeer *enetPeer);
    void onPeerReceived(ENetPeer *enetPeer, const void *p, quint64 len);

private slots:
    void onSocketActivated(QSocketDescriptor sd, QSocketNotifier::Type type);

private:
    State m_state;
    QUrl m_url;
    QSocketNotifier m_rxNotifier;
    ENetHost *m_enetHost;
    ENetPeer *m_enetPeer;
};

VOLCANO_NET_END

#endif // VOLCANO_NET_CLIENTWORLD_HPP
