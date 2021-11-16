//
//
#ifndef VOLCANO_SYSTEM_SESSION_HPP
#define VOLCANO_SYSTEM_SESSION_HPP

#include <QUdpSocket>

#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class Session: public QUdpSocket {
    Q_OBJECT

public:
    Session(QUdpSocket &socket, QObject *parent = nullptr);
    ~Session(void) override;

private:
    QUdpSocket &m_socket;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_SESSION_HPP
