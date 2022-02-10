//
//
#ifndef VOLCANO_SYSTEM_ENGINE_HPP
#define VOLCANO_SYSTEM_ENGINE_HPP

#include <QPair>
#include <QList>
#include <QUrl>
#include <QVersionNumber>
#include <QVector3D>
#include <QQuaternion>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQmlListProperty>

#include <Volcano/Game/Context.hpp>
#include <Volcano/Game/Manifest.hpp>
#include <Volcano/System/Common.hpp>
#include <Volcano/System/QmlQuaternionUtils.hpp>
#include <Volcano/System/NetworkAccessManagerFactory.hpp>

VOLCANO_SYSTEM_BEGIN

class Engine: public Game::Context {
    Q_OBJECT
    Q_PROPERTY(QVersionNumber engineVersion READ version)
    Q_PROPERTY(QmlQuaternionUtils *QuaternionUtils READ qmlQuaternionUtils)

public:
    Engine(QObject *parent = nullptr);
    ~Engine(void) override;

public:
    QVersionNumber version(void) const;
    bool init(const QUrl &url);
    bool shouldQuit(void) const;
    virtual TimePoint frame(void);
    Duration elapsed(void) override;
    Input::Service *inputService(void) override;
    void setInputService(Input::Service *p);
    Graphics::Service *graphicsService(void) override;
    void setGraphicsService(Graphics::Service *p);
    Physics::Service *physicsService(void) override;
    void setPhysicsService(Physics::Service *p);
    Sound::Service *soundService(void) override;
    void setSoundService(Sound::Service *p);

protected slots:
    void onStatusChanged(QQmlComponent::Status status);

protected:
    Q_INVOKABLE void quit(void);

private:
    QmlQuaternionUtils *qmlQuaternionUtils(void);

private:
    bool m_shouldQuit;
    QQmlEngine *m_qmlEngine;
    QQmlComponent *m_qmlComponent;
    Game::Manifest *m_gameManifest;
    Graphics::Service *m_graphicsService;
    Input::Service *m_inputService;
    Physics::Service *m_physicsService;
    Sound::Service *m_soundService;
    int m_frameCount;
    int m_frameCountPerSecond;
    TimePoint m_frameLast;
    TimePoint m_frameCountLast;
    Duration m_frameElapsedMin;
    Duration m_frameElapsed;
    QmlQuaternionUtils m_qmlQuaternionUtils;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_ENGINE_HPP
