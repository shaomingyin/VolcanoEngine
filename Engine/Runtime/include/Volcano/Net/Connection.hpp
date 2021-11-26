//
//
#ifndef VOLCANO_NET_CONNECTION_HPP
#define VOLCANO_NET_CONNECTION_HPP

#include <QString>
#include <QByteArray>
#include <QSocketNotifier>

#include <Volcano/Net/Common.hpp>
#include <Volcano/Net/Session.hpp>

VOLCANO_NET_BEGIN

class Connection: public Session {
    Q_OBJECT

public:
    Connection(QObject *parent = nullptr);
    ~Connection(void) override;

public:
    Q_INVOKABLE virtual bool connect(void);

protected:
    void onReceived(const QByteArray &data) override;

private:
    ENetHost *m_enetHost;
};

VOLCANO_NET_END

#endif // VOLCANO_NET_CONNECTION_HPP
