//
//
#include <QHostAddress>

#include <Volcano/System/Server.hpp>

VOLCANO_SYSTEM_BEGIN

Server::Server(QObject *parent):
    QObject(parent),
    m_tickTimer(0),
    m_host("0.0.0.0"),
    m_port(7788),
    m_gameWorld(nullptr)
{
    //m_stream.setByteOrder(QDataStream::LittleEndian);
    //m_stream.setDevice(&m_socket);

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

void Server::start(void)
{
    if (m_tickTimer > 0)
        return;

    if (!m_socket.bind(QHostAddress(m_host), m_port))
        return;

    connect(&m_socket, &QUdpSocket::readyRead, this, &Server::onReadyRead);
    //m_stream.resetStatus();

    m_tickTimer = startTimer(m_tickElapsedMin, Qt::PreciseTimer);
    m_tickCount = 0;
    m_tickCountPerSecond = 0;
    m_tickElapsedTimer.restart();

    emit runningChanged(true);
}

void Server::stop(void)
{
    if (m_tickTimer == 0)
        return;

    m_socket.close();

    killTimer(m_tickTimer);
    m_tickTimer = 0;
    m_tickCount = 0;
    m_tickCountPerSecond = 0;

    emit tpsChanged(0);
    emit runningChanged(false);
}

const QString &Server::host(void) const
{
    return m_host;
}

void Server::setHost(const QString &v)
{
    if (m_host == v)
        return;

    m_host = v;
    tryToRestart();
    hostChanged(v);
}

quint16 Server::port(void) const
{
    return m_port;
}

void Server::setPort(quint16 v)
{
    if (m_port == v)
        return;

    m_port = v;
    tryToRestart();
    emit portChanged(v);
}

Game::World *Server::gameWorld(void)
{
    return m_gameWorld;
}

void Server::setGameWorld(Game::World *p)
{
    if (m_gameWorld != p) {
        m_gameWorld = p;
        emit gameWorldChanged(p);
    }
}

void Server::timerEvent(QTimerEvent *evt)
{
    if (Q_LIKELY(evt->timerId() == m_tickTimer)) {
        auto elapsed = m_tickElapsedTimer.restart();
        tick(float(elapsed) / 1000.0f);
        m_tickCount += 1;
        return;
    }

    if (evt->timerId() == m_tickCountTimer && m_tickTimer > 0) {
        m_tickCountPerSecond = m_tickCount;
        m_tickCount = 0;
        emit tpsChanged(m_tickCountPerSecond);
    }
}

void Server::onReadyRead(void)
{
    auto size = m_socket.pendingDatagramSize();
    if (size < 1)
        return;
}

void Server::tick(float elapsed)
{
    if (Q_LIKELY(m_gameWorld != nullptr))
        m_gameWorld->tick(elapsed);

    // TODO
}

void Server::tryToRestart(void)
{
    if (m_tickTimer > 0) {
        stop();
        start();
    }
}

VOLCANO_SYSTEM_END
