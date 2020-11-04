//
//
#ifndef VOLCANO_GAME_VM_HPP
#define VOLCANO_GAME_VM_HPP

#include <QUrl>
#include <QTimerEvent>
#include <QElapsedTimer>
#include <QEventLoop>
#include <QQmlEngine>
#include <QQmlComponent>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

class VM: public QEventLoop
{
    Q_OBJECT

public:
    VM(QObject *parent = nullptr);
    VM(const QUrl &url, QObject *parent = nullptr);
    ~VM(void) override;

public:
    const QUrl &url(void) const;
    void setUrl(const QUrl &r);
    bool start(void);
    void stop(void);

signals:
    void error(const QString &msg);
    void ready(void);

protected:
    void timerEvent(QTimerEvent *event) override;

private slots:
    void statusChanged(QQmlComponent::Status v);

private:
    QUrl m_url;
    QQmlEngine *m_qmlEngine;
    QQmlComponent *m_qmlComponent;
    World *m_world;
    int m_frameTimer;
    int m_countTimer;
    int m_frameCountPerSecond;
    int m_frameCount;
    QElapsedTimer m_elapsedTimer;
};

VOLCANO_INLINE const QUrl &VM::url(void) const
{
    return m_url;
}

VOLCANO_INLINE void VM::setUrl(const QUrl &r)
{
    Q_ASSERT(m_qmlEngine == nullptr);
    Q_ASSERT(r.isValid());

    m_url = r;
}

VOLCANO_GAME_END

#endif // VOLCANO_GAME_VM_HPP
