//
//
#ifndef VOLCANO_SYSTEM_SERVER_HPP
#define VOLCANO_SYSTEM_SERVER_HPP

#include <list>

#include <QString>
#include <QUrl>
#include <QTimerEvent>
#include <QElapsedTimer>
#include <QUdpSocket>
#include <QDataStream>

#include <Volcano/Game/World.hpp>
#include <Volcano/System/Common.hpp>
#include <Volcano/System/Session.hpp>

VOLCANO_SYSTEM_BEGIN

class Server: public Game::World {
    Q_OBJECT
    Q_PROPERTY(int tps READ tps NOTIFY tpsChanged)
    Q_PROPERTY(int tpsMax READ tpsMax WRITE setTpsMax NOTIFY tpsMaxChanged)
    Q_PROPERTY(bool running READ isRunning WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged)

public:
    Server(QObject *parent = nullptr);
    ~Server(void) override;

public:
    int tps(void) const;
    int tpsMax(void) const;
    void setTpsMax(int v);
    bool isRunning(void) const;
    void setRunning(bool v);
    Q_INVOKABLE void start(void);
    Q_INVOKABLE void stop(void);
    QString host(void) const;
    void setHost(QString v);
    quint16 port(void) const;
    void setPort(quint16 v);

signals:
    void tpsChanged(int v);
    void tpsMaxChanged(int v);
    void runningChanged(bool v);
    void hostChanged(QString v);
    void portChanged(quint16 v);

protected:
    void timerEvent(QTimerEvent *evt) override;
    void tick(float elapsed) override;

private:
    void tryToRestart(void);

private:
    using SessionList = std::list<Session>;

private:
    int m_tickTimer;
    int m_tickElapsedMin;
    int m_tickCount;
    int m_tickCountTimer;
    int m_tickCountPerSecond;
    QElapsedTimer m_tickElapsedTimer;
    QString m_host;
    quint16 m_port;
    QUdpSocket m_socket;
    QDataStream m_txStream;
    QByteArray m_txBuffer;
    SessionList m_sessionList;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_SERVER_HPP
