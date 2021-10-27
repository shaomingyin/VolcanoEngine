//
//
#include <QHostAddress>

#include <Volcano/System/Server.hpp>

VOLCANO_SYSTEM_BEGIN

Server::Server(QObject *parent):
    QObject(parent),
    m_isStarted(false),
    m_host("0.0.0.0"),
    m_port(7788),
    m_gameWorld(nullptr)
{
    m_stream.setByteOrder(QDataStream::LittleEndian);
    m_stream.setDevice(&m_socket);
}

Server::~Server(void)
{
}

bool Server::isStarted(void) const
{
    return m_isStarted;
}

void Server::setStart(bool v)
{
    if (m_isStarted != v) {
        if (v)
            start();
        else
            stop();
    }
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

    if (m_isStarted) {
        stop();
        start();
    }

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

    if (m_isStarted) {
        stop();
        start();
    }

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

void Server::start(void)
{
    Q_ASSERT(!m_socket.isOpen());

    if (!m_socket.bind(QHostAddress(m_host), m_port))
        return;

    connect(&m_socket, &QUdpSocket::readyRead, this, &Server::onReadyRead);
    m_stream.resetStatus();

    m_isStarted = true;
    emit startedChanged(true);
}

void Server::stop(void)
{
    Q_ASSERT(m_socket.isOpen());

    m_socket.close();

    m_isStarted = false;
    emit startedChanged(false);
}

void Server::onReadyRead(void)
{
    auto size = m_socket.bytesAvailable();
    if (size < 1)
        return;
}

VOLCANO_SYSTEM_END
