//
//
#ifndef VOLCANO_SYSTEM_CLIENT_HPP
#define VOLCANO_SYSTEM_CLIENT_HPP

#include <QUrl>

#include <Volcano/System/Common.hpp>
#include <Volcano/System/GameWindow.hpp>
#include <Volcano/System/Context.hpp>

VOLCANO_SYSTEM_BEGIN

class Client: public Context {
    Q_OBJECT
    Q_PROPERTY(bool start READ isStarted WRITE setStart NOTIFY startedChanged)
    Q_PROPERTY(QUrl serverUrl READ serverUrl WRITE setServerUrl NOTIFY serverUrlChanged)

public:
    Client(QObject *parent = nullptr);
    ~Client(void) override;

public:
    bool isStarted(void) const;
    void setStart(bool v);
    const QUrl &serverUrl(void) const;
    void setServerUrl(const QUrl &v);

signals:
    void startedChanged(bool v);
    void serverUrlChanged(const QUrl &v);

private:
    bool m_isStarted;
    QUrl m_serverUrl;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_CLIENT_HPP
