//
//
#include <Volcano/Net/Socket.hpp>

VOLCANO_NET_BEGIN

Socket::Socket(QObject *parent):
    QObject(parent),
    m_readNotifier(QSocketNotifier::Read),
    m_writeNotifier(QSocketNotifier::Write),
    m_exceptionNotifier(QSocketNotifier::Exception),
    m_state(STATE_DISCONNECTED)
{
    connect(&m_readNotifier, &QSocketNotifier::activated, this, &Socket::onNotifierActivated);
    connect(&m_writeNotifier, &QSocketNotifier::activated, this, &Socket::onNotifierActivated);
    connect(&m_exceptionNotifier, &QSocketNotifier::activated, this, &Socket::onNotifierActivated);
}

Socket::~Socket(void)
{
}

Socket::State Socket::state(void) const
{
    return m_state;
}

bool Socket::init(qintptr sk)
{
    Q_ASSERT(!m_readNotifier.isValid());
    Q_ASSERT(!m_writeNotifier.isValid());
    Q_ASSERT(!m_exceptionNotifier.isValid());

    m_readNotifier.setSocket(sk);
    m_writeNotifier.setSocket(sk);
    m_exceptionNotifier.setSocket(sk);

    m_readNotifier.setEnabled(true);
    m_writeNotifier.setEnabled(true);
    m_exceptionNotifier.setEnabled(true);

    return true;
}

void Socket::shutdown(void)
{
    Q_ASSERT(m_readNotifier.isValid());
    Q_ASSERT(m_writeNotifier.isValid());
    Q_ASSERT(m_exceptionNotifier.isValid());

    m_readNotifier.setEnabled(false);
    m_writeNotifier.setEnabled(false);
    m_exceptionNotifier.setEnabled(false);
}

void Socket::setState(State v)
{
    if (m_state != v) {
        m_state = v;
        emit stateChanged(v);
    }
}

void Socket::onNotifierActivated(QSocketDescriptor socket, QSocketNotifier::Type type)
{
    emit onActivated(type);
}

VOLCANO_NET_END
