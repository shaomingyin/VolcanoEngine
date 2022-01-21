//
//
#ifndef VOLCANO_SYSTEM_CORE_HPP
#define VOLCANO_SYSTEM_CORE_HPP

#include <QUrl>
#include <QQmlEngine>
#include <QQmlComponent>

#include <Volcano/Game/Context.hpp>
#include <Volcano/Game/Manifest.hpp>
#include <Volcano/System/Common.hpp>
#include <Volcano/System/NetworkAccessManagerFactory.hpp>

VOLCANO_SYSTEM_BEGIN

class Core: public Game::Context {
    Q_OBJECT

public:
    Core(QObject *parent = nullptr);
    ~Core(void) override;

public:
    bool init(const QUrl &url);
    TimePoint frame(void);
    virtual QStringList enumateGraphicsServices(void) override;
    virtual bool useGraphicsService(const QString &name) override;
    Graphics::Service *graphicsService(void) override;
    Input::Service *inputService(void) override;
    virtual QStringList enumatePhysicsServices(void) override;
    virtual bool usePhysicsService(const QString &name) override;
    Physics::Service *physicsService(void) override;
    virtual QStringList enumateSoundServices(void) override;
    virtual bool useSoundService(const QString &name) override;
    Sound::Service *soundService(void) override;

protected slots:
    void onStatusChanged(QQmlComponent::Status status);

private:
    QQmlEngine *m_qmlEngine;
    QQmlComponent *m_qmlComponent;
    NetworkAccessManagerFactory *m_networkAccessManagerFactor;
    Game::Manifest *m_gameManifest;
    int m_frameCount;
    int m_frameCountPerSecond;
    TimePoint m_frameLast;
    TimePoint m_frameCountLast;
    Duration m_frameElapsedMin;
    Graphics::Service *m_graphicsService;
    Input::Service *m_inputService;
    Physics::Service *m_physicsService;
    Sound::Service *m_soundService;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_CORE_HPP
