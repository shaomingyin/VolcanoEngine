//
//
#ifndef VOLCANO_SYSTEM_CORE_HPP
#define VOLCANO_SYSTEM_CORE_HPP

#include <QPair>
#include <QList>
#include <QUrl>
#include <QVersionNumber>
#include <QVector3D>
#include <QQuaternion>
#include <QMetaObject>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQmlListProperty>

#include <Volcano/Game/Context.hpp>
#include <Volcano/Game/Manifest.hpp>
#include <Volcano/System/Common.hpp>
#include <Volcano/System/QuaternionUtils.hpp>
#include <Volcano/System/NetworkAccessManagerFactory.hpp>

VOLCANO_SYSTEM_BEGIN

using ServiceList = QList<QMetaObject *>;

class Core: public Game::Context {
    Q_OBJECT
    Q_PROPERTY(QVersionNumber engineVersion READ version)
    Q_PROPERTY(QQmlListProperty< graphicsServiceList READ graphicsServiceList NOTIFY graphicsServiceListChanged)
    Q_PROPERTY(QString graphicsService READ graphicsServiceName WRITE setGraphicsService NOTIFY graphicsServiceChanged)
    Q_PROPERTY(QStringList physicsServiceList READ physicsServiceList NOTIFY physicsServiceListChanged)
    Q_PROPERTY(QString physicsService READ physicsServiceName WRITE setPhysicsService NOTIFY physicsServiceChanged)
    Q_PROPERTY(QStringList soundServiceList READ soundServiceList NOTIFY soundServiceListChanged)
    Q_PROPERTY(QString soundService READ soundServiceName WRITE setSoundService NOTIFY soundServiceChanged)
    Q_PROPERTY(QuaternionUtils *QuaternionUtils READ quaternionUtils)

public:
    Core(QObject *parent = nullptr);
    ~Core(void) override;

public:
    virtual bool init(const QUrl &url);
    bool shouldQuit(void) const;
    TimePoint frame(void);
    bool registerService(QMetaObject *p);
    bool unregisterService(QMetaObject *p);
    Input::Service *inputService(void) override;
    const ServiceList &graphicsServiceList(void) const;
    Graphics::Service *graphicsService(void) override;
    QString graphicsServiceName(void) const;
    bool setGraphicsService(const QString &name);
    ServiceList physicsServiceList(void);
    Physics::Service *physicsService(void) override;
    QString physicsServiceName(void) const;
    bool setPhysicsService(const QString &name);
    ServiceList soundServiceList(void);
    Sound::Service *soundService(void) override;
    QString soundServiceName(void) const;
    virtual bool setSoundService(const QString &name);

signals:
    void graphicsServiceListChanged(QStringList list);
    void graphicsServiceChanged(QString name);
    void physicsServiceListChanged(QStringList list);
    void physicsServiceChanged(QString name);
    void soundServiceListChanged(QStringList list);
    void soundServiceChanged(QString name);

protected slots:
    void onStatusChanged(QQmlComponent::Status status);

private:
    QVersionNumber version(void) const;
    QuaternionUtils *quaternionUtils(void);
    Q_INVOKABLE void quit(void);

private:
    bool m_shouldQuit;
    ServiceList m_serviceList;
    ServiceList m_graphicsServiceList;
    ServiceList m_physicsServiceList;
    ServiceList m_soundServiceList;
    QQmlEngine *m_qmlEngine;
    QQmlComponent *m_qmlComponent;
    Game::Manifest *m_gameManifest;
    Graphics::Service *m_graphicsService;
    Input::Service *m_inputService;
    Physics::Service *m_physicsService;
    Sound::Service *m_soundService;
    int m_frameTimer;
    int m_frameCountTimer;
    int m_frameCount;
    int m_frameCountPerSecond;
    TimePoint m_frameLast;
    TimePoint m_frameCountLast;
    Duration m_frameElapsedMin;
    QuaternionUtils m_quaternionUtils;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_CORE_HPP
