//
//
#include <Volcano/System/Session.hpp>

VOLCANO_SYSTEM_BEGIN

Session::Session(QObject *parent):
    QUdpSocket(parent)
{
}

Session::~Session(void)
{
}

VOLCANO_SYSTEM_END
