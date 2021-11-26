//
//
#include <Volcano/Net/Server.hpp>

VOLCANO_NET_BEGIN

Server::Server(QObject *parent):
    Socket(parent),
    m_host("any"),
    m_port(DEFAULT_PORT),
    m_maxSession(DEFAULT_MAX_SESSION),
    m_enetHost(nullptr)
{
}

Server::~Server(void)
{
    if (m_enetHost != nullptr)
        enet_host_destroy(m_enetHost);
}

const QString &Server::host(void) const
{
    return m_host;
}

void Server::setHost(const QString &v)
{
    if (m_host == v)
        return;

    // TODO

    m_host = v;
    emit hostChanged(v);
}

quint16 Server::port(void) const
{
    return m_port;
}

void Server::setPort(quint16 v)
{
    if (m_port == v)
        return;

    // TODO

    m_port = v;
    emit portChanged(v);
}

int Server::maxSession(void) const
{
    return m_maxSession;
}

void Server::setMaxSession(int v)
{
    int tmp = qBound(1, v, 256);
    if (m_maxSession == tmp)
        return;

    // TODO

    m_maxSession = tmp;
    emit maxSessionChanged(tmp);
}

bool Server::isRunning(void) const
{
    return (m_enetHost != nullptr);
}

void Server::setRunning(bool v)
{
    if (m_enetHost != nullptr) {
        if (!v)
            stop();
    } else {
        if (v)
            start();
    }
}

bool Server::start(void)
{
    if (isRunning())
        return true;

    ENetAddress addr;
    if (m_host.compare("any", Qt::CaseInsensitive) == 0)
        addr.host = ENET_HOST_ANY;
    else
        enet_address_set_host(&addr, qPrintable(m_host));
    addr.port = m_port;

    m_enetHost = enet_host_create(&addr, m_maxSession, 0, 0, 0);
    if (m_enetHost == nullptr)
        return false;

    if (!init(m_enetHost->socket)) {
        enet_host_destroy(m_enetHost);
        m_enetHost = nullptr;
        return false;
    }

    emit runningChanged(true);

    return true;
}

void Server::stop(void)
{
    if (!isRunning())
        return;

    shutdown();

    enet_host_destroy(m_enetHost);
    m_enetHost = nullptr;

    emit runningChanged(false);
}

const SessionList &Server::sessions(void) const
{
    return m_sessionList;
}

QQmlListProperty<Session> Server::qmlSessions(void)
{
    return { this, this,
        &Server::qmlSessionCount,
        &Server::qmlSessionAt };
}

void Server::onActivated(QSocketNotifier::Type type)
{
    Q_ASSERT(m_enetHost != nullptr);

    ENetEvent evt;
    int ret = enet_host_service(m_enetHost, &evt, 0);
    if (ret < 1)
        return;

    Session *session = nullptr;
    switch (evt.type) {
    case ENET_EVENT_TYPE_CONNECT:
        session = newSession();
        session->init(evt.peer);
        m_sessionList.append(session);
        break;
    case ENET_EVENT_TYPE_DISCONNECT:
        session = Session::fromENetPeer(evt.peer);
        m_sessionList.removeOne(session);
        delete session;
        break;
    case ENET_EVENT_TYPE_RECEIVE:
        Session::fromENetPeer(evt.peer)->onReceived(QByteArray::fromRawData(
            (const char *)(evt.packet->data), evt.packet->dataLength));
        break;
    case ENET_EVENT_TYPE_NONE:
    default:
        break;
    }
}

qsizetype Server::qmlSessionCount(QQmlListProperty<Session> *list)
{
    return reinterpret_cast<Server *>(list->data)->m_sessionList.count();
}

Session *Server::qmlSessionAt(QQmlListProperty<Session> *list, qsizetype index)
{
    return reinterpret_cast<Server *>(list->data)->m_sessionList.at(index);
}

VOLCANO_NET_END
