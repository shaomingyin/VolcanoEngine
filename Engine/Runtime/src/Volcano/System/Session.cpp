//
//
#include <Volcano/System/Session.hpp>

VOLCANO_SYSTEM_BEGIN

Session::Session(QUdpSocket &socket, QObject *parent):
    QUdpSocket(parent),
    m_socket(socket)
{
}

Session::~Session(void)
{
}

VOLCANO_SYSTEM_END
