//
//
#include <memory>

#include <QScopeGuard>
#include <QDataStream>
#include <QHostAddress>

#include <Volcano/Net/Server.hpp>

VOLCANO_NET_BEGIN

Server::Server(QObject *parent):
    QObject(parent),
    m_tickTimer(0),
    m_maxSession(16),
    m_url("volcano://any"),
    m_gameActorSet(nullptr),
    m_rxNotifier(QSocketNotifier::Read),
    m_enetHost(nullptr)
{
    //m_stream.setByteOrder(QDataStream::LittleEndian);
    //m_stream.setDevice(&m_socket);

    auto onActivated = [this](QSocketDescriptor sd, QSocketNotifier::Type type) {
        pollENet();
    };

    connect(&m_rxNotifier, &QSocketNotifier::activated, onActivated);

    setTpsMax(20);
    m_tickCountTimer = startTimer(1000);
}

Server::~Server(void)
{
}

int Server::tps(void) const
{
    return m_tickCountPerSecond;
}

int Server::tpsMax(void) const
{
    Q_ASSERT(m_tickElapsedMin > 0);

    return 1000 / m_tickElapsedMin;
}

void Server::setTpsMax(int v)
{
    int tickElapsedMin = 1000 / qBound(1, v, 1000);
    if (tickElapsedMin == m_tickElapsedMin)
        return;

    m_tickElapsedMin = tickElapsedMin;
    tryToRestart();
    emit tpsMaxChanged(m_tickElapsedMin);
}

bool Server::isRunning(void) const
{
    return (m_tickTimer > 0);
}

void Server::setRunning(bool v)
{
    bool isRunning = m_tickTimer > 0;
    if (isRunning != v) {
        if (v)
            start();
        else
            stop();
    }
}

int Server::maxSession(void) const
{
    return m_maxSession;
}

void Server::setMaxSession(int v)
{
    int tmp = qBound(1, v, 256);
    if (m_maxSession != tmp) {
        m_maxSession = tmp;
        tryToRestart();
        emit maxSessionChanged(tmp);
    }
}

const QUrl &Server::url(void) const
{
    return m_url;
}

void Server::setUrl(const QUrl &v)
{
    if (m_url!= v) {
        m_url = v;
        tryToRestart();
        emit urlChanged(v);
    }
}

bool Server::start(void)
{
    if (m_enetHost != nullptr)
        return true;

    if (m_url.scheme().compare("volcano", Qt::CaseInsensitive))
        return false;

    ENetAddress addr;

    QString host = m_url.host();
    if (host.compare("any", Qt::CaseInsensitive))
        enet_address_set_host(&addr, qPrintable(m_url.host()));
    else
        addr.host = ENET_HOST_ANY;

    addr.port = m_url.port(DEFAULT_PORT);

    Q_ASSERT(m_enetHost == nullptr);
    m_enetHost = enet_host_create(&addr, m_maxSession, 0, 0, 0);
    if (m_enetHost == nullptr)
        return false;

    m_rxNotifier.setSocket(m_enetHost->socket);
    m_rxNotifier.setEnabled(true);
    //m_stream.resetStatus();

    m_tickTimer = startTimer(m_tickElapsedMin, Qt::PreciseTimer);
    m_tickCount = 0;
    m_tickCountPerSecond = 0;
    m_tickElapsedTimer.restart();

    emit runningChanged(true);

    return true;
}

void Server::stop(void)
{
    if (m_enetHost == nullptr)
        return;

    Q_ASSERT(m_rxNotifier.isValid());
    Q_ASSERT(m_rxNotifier.isEnabled());
    m_rxNotifier.setSocket(-1);
    m_rxNotifier.setEnabled(false);

    Q_ASSERT(m_enetHost != nullptr);
    enet_host_destroy(m_enetHost);
    m_enetHost = nullptr;

    killTimer(m_tickTimer);
    m_tickTimer = 0;
    m_tickCount = 0;
    m_tickCountPerSecond = 0;

    emit tpsChanged(0);
    emit runningChanged(false);
}

Game::ActorSet *Server::gameActorSet(void)
{
    return m_gameActorSet;
}

void Server::setGameActorSet(Game::ActorSet *p)
{
    if (m_gameActorSet == p)
        return;

    if (m_gameActorSet != nullptr) {
        // TODO
    }

    m_gameActorSet = p;

    if (m_gameActorSet != nullptr) {
        // TODO
    }

    emit gameActorSetChanged(p);
}

const SessionList &Server::sessions(void) const
{
    return m_sessions;
}

QQmlListProperty<Session> Server::qmlSessions(void)
{
    return { this, this,
        &Server::qmlSessionCount,
        &Server::qmlSessionAt };
}

qsizetype Server::sessionCount(void) const
{
    return m_sessions.count();
}

Session *Server::sessionAt(qsizetype index)
{
    return m_sessions[index];
}

void Server::timerEvent(QTimerEvent *evt)
{
    if (Q_LIKELY(evt->timerId() == m_tickTimer)) {
        tick(std::chrono::milliseconds(m_tickElapsedTimer.restart()));
        m_tickCount += 1;
        return;
    }

    if (evt->timerId() == m_tickCountTimer && m_tickTimer > 0) {
        m_tickCountPerSecond = m_tickCount;
        m_tickCount = 0;
        emit tpsChanged(m_tickCountPerSecond);
    }
}

void Server::tick(Duration elapsed)
{
    pollENet();

    if (m_gameActorSet != nullptr)
        m_gameActorSet->tick(elapsed);

    // TODO
}

qsizetype Server::qmlSessionCount(QQmlListProperty<Session> *list)
{
    auto server = reinterpret_cast<Server *>(list->data);
    return server->sessionCount();
}

Session *Server::qmlSessionAt(QQmlListProperty<Session> *list, qsizetype index)
{
    auto server = reinterpret_cast<Server *>(list->data);
    return server->sessionAt(index);
}

void Server::tryToRestart(void)
{
    if (m_tickTimer > 0) {
        stop();
        start();
    }
}

void Server::onPeerConnected(ENetPeer *enetPeer)
{
    auto it(findSesssion(enetPeer));
    if (it == m_sessions.end()) {
        auto p = new Session(enetPeer, this);
        m_sessions.append(p);
        // TODO
    }
}

void Server::onPeerDisconnected(ENetPeer *enetPeer)
{
    auto it(findSesssion(enetPeer));
    if (it != m_sessions.end()) {
        Q_ASSERT(enetPeer == (*it)->toENetPeer());
        m_sessions.erase(it);
    }
}

void Server::onPeerReceived(ENetPeer *enetPeer, const void *p, quint64 len)
{
    if (isRunning()) {
        Q_ASSERT(m_enetHost != nullptr);
        Q_ASSERT(findSesssion(enetPeer) != m_sessions.end());
        auto data = QByteArray::fromRawData((const char *)p, len);
        Session::handleReceived(enetPeer, data);
    }
}

void Server::pollENet(void)
{
    Q_ASSERT(m_enetHost != nullptr);

    ENetEvent evt;
    int ret = enet_host_service(m_enetHost, &evt, 0);
    if (ret > 0) {
        switch (evt.type) {
        case ENET_EVENT_TYPE_RECEIVE:
            onPeerReceived(evt.peer, evt.packet->data, evt.packet->dataLength);
            enet_packet_destroy(evt.packet);
            break;
        case ENET_EVENT_TYPE_CONNECT:
            onPeerConnected(evt.peer);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            onPeerDisconnected(evt.peer);
            break;
        default:
            break;
        }
    }
}

SessionList::iterator Server::findSesssion(ENetPeer *enetPeer)
{
    Q_ASSERT(enetPeer != nullptr);

    SessionList::iterator it;
    for (it = m_sessions.begin(); it != m_sessions.end(); ++it) {
        if ((*it)->toENetPeer() == enetPeer)
            break;
    }

    return it;
}

VOLCANO_NET_END
