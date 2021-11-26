//
//
#ifndef VOLCANO_SYSTEM_CLIENT_HPP
#define VOLCANO_SYSTEM_CLIENT_HPP

#include <QBuffer>
#include <QDataStream>
#include <QObject>

#include <Volcano/Net/Connection.hpp>
#include <Volcano/Graphics/Viewable.hpp>
#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class Client: public Graphics::Viewable {
    Q_OBJECT
    Q_PROPERTY(Net::Connection *connection READ connection)

public:
    Client(QObject *parent = nullptr);
    ~Client(void) override;

public:
    Net::Connection *connection(void);
    void buildVisibleSet(Graphics::VisibleSet &out, Graphics::Camera &cam) override;

signals:
    void deviceChanged(QIODevice *p);

private slots:
    void onDataReceived(const QByteArray &data);

private:
    Net::Connection m_connection;
    QBuffer m_rxBuffer;
    QDataStream m_rxStream;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_CLIENT_HPP
