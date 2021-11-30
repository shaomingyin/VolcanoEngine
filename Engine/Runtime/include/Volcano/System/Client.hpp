//
//
#ifndef VOLCANO_SYSTEM_CLIENT_HPP
#define VOLCANO_SYSTEM_CLIENT_HPP

#include <QUrl>
#include <QSocketNotifier>
#include <QSocketDescriptor>

#include <Volcano/Graphics/Viewable.hpp>
#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class Client: public Graphics::Viewable {
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
    Client(QObject *parent = nullptr);
    ~Client(void) override;

public:
    State state(void) const;
    bool isEnabled(void) const;
    void setEnabled(bool v);
    const QUrl &url(void) const;
    void setUrl(const QUrl &v);
    Q_INVOKABLE bool enable(void);
    Q_INVOKABLE void disable(void);
    void buildVisibleSet(Graphics::VisibleSet &out, Graphics::Camera &cam) override;

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

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_CLIENT_HPP
