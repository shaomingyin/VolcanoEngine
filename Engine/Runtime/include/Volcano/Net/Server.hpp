//
//
#ifndef VOLCANO_NET_SERVER_HPP
#define VOLCANO_NET_SERVER_HPP

#include <QList>
#include <QString>
#include <QQmlListProperty>

#include <Volcano/Net/Common.hpp>
#include <Volcano/Net/Socket.hpp>
#include <Volcano/Net/Session.hpp>

VOLCANO_NET_BEGIN

using SessionList = QList<Session *>;

class Server: public Socket {
    Q_OBJECT
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(int maxSession READ maxSession WRITE setMaxSession NOTIFY maxSessionChanged)
    Q_PROPERTY(bool running READ isRunning WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(QQmlListProperty<Session> sessions READ qmlSessions)

public:
    Server(QObject *parent = nullptr);
    ~Server(void) override;

public:
    const QString &host(void) const;
    void setHost(const QString &v);
    quint16 port(void) const;
    void setPort(quint16 v);
    int maxSession(void) const;
    void setMaxSession(int v);
    bool isRunning(void) const;
    void setRunning(bool v);
    virtual bool start(void);
    virtual void stop(void);
    const SessionList &sessions(void) const;
    QQmlListProperty<Session> qmlSessions(void);

signals:
    void hostChanged(const QString &v);
    void portChanged(quint16 v);
    void maxSessionChanged(int v);
    void runningChanged(bool v);

protected:
    void onActivated(QSocketNotifier::Type type) override;
    virtual Session *newSession(void) = 0;

private:
    static qsizetype qmlSessionCount(QQmlListProperty<Session> *list);
    static Session *qmlSessionAt(QQmlListProperty<Session> *list, qsizetype index);

private:
    QString m_host;
    quint16 m_port;
    int m_maxSession;
    ENetHost *m_enetHost;
    SessionList m_sessionList;
};

VOLCANO_NET_END

#endif // VOLCANO_NET_SERVER_HPP
