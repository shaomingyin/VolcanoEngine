//
//
#ifndef VOLCANO_NET_SOCKET_HPP
#define VOLCANO_NET_SOCKET_HPP

#include <QObject>
#include <QSocketNotifier>
#include <QSocketDescriptor>

#include <Volcano/Net/Common.hpp>

VOLCANO_NET_BEGIN

class Socket: public QObject {
    Q_OBJECT
    Q_PROPERTY(State state READ state NOTIFY stateChanged)

public:
    enum State {
        STATE_DISCONNECTED = 0,
        STATE_CONNECTING,
        STATE_CONNECTED,
        STATE_LISTENING,
        STATE_ERROR
    };
    Q_ENUM(State)

public:
    Socket(QObject *parent = nullptr);
    ~Socket(void) override;

public:
    State state(void) const;

signals:
    void stateChanged(State v);

protected:
    bool init(qintptr sk);
    void shutdown(void);
    void setState(State v);
    virtual void onActivated(QSocketNotifier::Type type) = 0;

private slots:
    void onNotifierActivated(QSocketDescriptor socket, QSocketNotifier::Type type);

private:
    State m_state;
    QSocketNotifier m_readNotifier;
    QSocketNotifier m_writeNotifier;
    QSocketNotifier m_exceptionNotifier;
};

VOLCANO_NET_END

#endif // VOLCANO_NET_SOCKET_HPP
