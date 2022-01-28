//
//
#ifndef VOLCANO_SYSTEM_ENGINE_HPP
#define VOLCANO_SYSTEM_ENGINE_HPP

#include <QUrl>
#include <QVersionNumber>
#include <QVector3D>
#include <QQuaternion>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>

#include <Volcano/Game/Context.hpp>
#include <Volcano/Game/Manifest.hpp>
#include <Volcano/System/Common.hpp>
#include <Volcano/System/QuaternionUtils.hpp>
#include <Volcano/System/NetworkAccessManagerFactory.hpp>

VOLCANO_SYSTEM_BEGIN

class Engine: public Game::Context {
    Q_OBJECT
    Q_PROPERTY(QVersionNumber engineVersion READ version)
    Q_PROPERTY(QStringList graphicsServiceList READ graphicsServiceList NOTIFY graphicsServiceListChanged)
    Q_PROPERTY(QString graphicsService READ graphicsServiceName WRITE setGraphicsService NOTIFY graphicsServiceChanged)
    Q_PROPERTY(QStringList physicsServiceList READ physicsServiceList NOTIFY physicsServiceListChanged)
    Q_PROPERTY(QString physicsService READ physicsServiceName WRITE setPhysicsService NOTIFY physicsServiceChanged)
    Q_PROPERTY(QStringList soundServiceList READ soundServiceList NOTIFY soundServiceListChanged)
    Q_PROPERTY(QString soundService READ soundServiceName WRITE setSoundService NOTIFY soundServiceChanged)
    Q_PROPERTY(QuaternionUtils *QuaternionUtils READ quaternionUtils)

public:
    Engine(QObject *parent = nullptr);
    ~Engine(void) override;

public:
    bool init(const QUrl &url);
    bool shouldQuit(void) const;
    TimePoint frame(void);
    Input::Service *inputService(void) override;
    QStringList graphicsServiceList(void);
    Graphics::Service *graphicsService(void) override;
    QString graphicsServiceName(void) const;
    bool setGraphicsService(const QString &name);
    QStringList physicsServiceList(void);
    Physics::Service *physicsService(void) override;
    QString physicsServiceName(void) const;
    bool setPhysicsService(const QString &name);
    QStringList soundServiceList(void);
    Sound::Service *soundService(void) override;
    QString soundServiceName(void) const;
    virtual bool setSoundService(const QString &name);
    Q_INVOKABLE void quit(void);

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

private:
    bool m_shouldQuit;
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

#endif // VOLCANO_SYSTEM_ENGINE_HPP
