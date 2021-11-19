//
//
#ifndef VOLCANO_SYSTEM_SERVER_HPP
#define VOLCANO_SYSTEM_SERVER_HPP

#include <list>

#include <QString>
#include <QUrl>
#include <QObject>
#include <QTimerEvent>
#include <QElapsedTimer>
#include <QUdpSocket>
#include <QDataStream>

#include <Volcano/Game/World.hpp>
#include <Volcano/System/Common.hpp>
#include <Volcano/System/Session.hpp>

VOLCANO_SYSTEM_BEGIN

class Server: public QObject {
    Q_OBJECT
    Q_PROPERTY(int tps READ tps NOTIFY tpsChanged)
    Q_PROPERTY(int tpsMax READ tpsMax WRITE setTpsMax NOTIFY tpsMaxChanged)
    Q_PROPERTY(bool running READ isRunning WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(QUrl bind READ bind WRITE setBind NOTIFY bindChanged)
    Q_PROPERTY(QString loopback READ loopback WRITE setLoopback NOTIFY loopbackChanged)
    Q_PROPERTY(Game::World *gameWorld READ gameWorld WRITE setGameWorld NOTIFY gameWorldChanged)

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
    const QUrl &bind(void) const;
    void setBind(const QUrl &v);
    QString loopback(void) const;
    void setLoopback(const QString &v);
    Game::World *gameWorld(void);
    void setGameWorld(Game::World *p);

signals:
    void tpsChanged(int v);
    void tpsMaxChanged(int v);
    void runningChanged(bool v);
    void hostChanged(const QString &v);
    void portChanged(quint16 v);
    void gameWorldChanged(Game::World *p);

protected:
    void timerEvent(QTimerEvent *evt) override;

private slots:
    void onReadyRead(void);

private:
    void tick(float elapsed);
    void tryToRestart(void);

private:
    using SessionList = std::list<Session *>;

private:
    int m_tickTimer;
    int m_tickElapsedMin;
    int m_tickCount;
    int m_tickCountTimer;
    int m_tickCountPerSecond;
    QElapsedTimer m_tickElapsedTimer;
    QUrl m_bind;
    QString m_loopback;
    QUdpSocket m_socket;
    QDataStream m_txStream;
    QByteArray m_txBuffer;
    SessionList m_sessionList;
    Game::World *m_gameWorld;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_SERVER_HPP
