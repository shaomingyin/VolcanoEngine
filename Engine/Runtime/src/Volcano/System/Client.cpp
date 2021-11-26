//
//
#include <Volcano/System/Protocol.hpp>
#include <Volcano/System/Client.hpp>

VOLCANO_SYSTEM_BEGIN

Client::Client(QObject *parent):
    Graphics::Viewable(parent)
{
    m_rxStream.setVersion(QDataStream::Qt_6_0);
    m_rxStream.setByteOrder(QDataStream::LittleEndian);
    m_rxStream.setDevice(&m_rxBuffer);

    connect(&m_connection, &Net::Connection::dataReceived, this, &Client::onDataReceived);
}

Client::~Client(void)
{
}

Net::Connection *Client::connection(void)
{
    return &m_connection;
}

void Client::buildVisibleSet(Graphics::VisibleSet &out, Graphics::Camera &cam)
{
}

void Client::onDataReceived(const QByteArray &data)
{
    m_rxBuffer.close();
    m_rxBuffer.setData(data);
    m_rxBuffer.open(QBuffer::ReadOnly);
    m_rxStream.resetStatus();

    // TODO
}

VOLCANO_SYSTEM_END
