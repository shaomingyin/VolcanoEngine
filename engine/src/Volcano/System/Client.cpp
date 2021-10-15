//
//
#include <Volcano/System/Client.hpp>

VOLCANO_SYSTEM_BEGIN

Client::Client(QObject *parent):
    Context(parent)
{
}

Client::~Client(void)
{
}

bool Client::isStarted(void) const
{
    return m_isStarted;
}

void Client::setStart(bool v)
{
    if (m_isStarted == v)
        return;


    // TODO connect to server url...

    m_isStarted = v;
    emit startedChanged(v);
}

const QUrl &Client::serverUrl(void) const
{
    return m_serverUrl;
}

void Client::setServerUrl(const QUrl &v)
{
    if (m_serverUrl == v)
        return;

    if (m_isStarted) {
        // TODO start reconnection to the new server.
    }

    m_serverUrl = v;
    emit serverUrlChanged(v);
}

VOLCANO_SYSTEM_END
