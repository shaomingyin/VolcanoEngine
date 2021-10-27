//
//
#ifndef VOLCANO_SYSTEM_SERVER_HPP
#define VOLCANO_SYSTEM_SERVER_HPP

#include <QString>
#include <QObject>
#include <QUdpSocket>
#include <QDataStream>

#include <Volcano/Game/World.hpp>
#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class Server: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool start READ isStarted WRITE setStart NOTIFY startedChanged)
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(Game::World *gameWorld READ gameWorld WRITE setGameWorld NOTIFY gameWorldChanged)

public:
    Server(QObject *parent = nullptr);
    ~Server(void) override;

public:
    bool isStarted(void) const;
    void setStart(bool v);
    const QString &host(void) const;
    void setHost(const QString &v);
    quint16 port(void) const;
    void setPort(quint16 v);
    Game::World *gameWorld(void);
    void setGameWorld(Game::World *p);

signals:
    void startedChanged(bool v);
    void hostChanged(const QString &v);
    void portChanged(quint16 v);
    void gameWorldChanged(Game::World *p);

private:
    void start(void);
    void stop(void);

private slots:
    void onReadyRead(void);

private:
    bool m_isStarted;
    QString m_host;
    quint16 m_port;
    QUdpSocket m_socket;
    QDataStream m_stream;
    Game::World *m_gameWorld;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_SERVER_HPP
