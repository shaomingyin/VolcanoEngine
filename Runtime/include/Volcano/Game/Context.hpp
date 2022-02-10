//
//
#ifndef VOLCANO_GAME_CONTEXT_HPP
#define VOLCANO_GAME_CONTEXT_HPP

#include <QString>
#include <QVersionNumber>
#include <QQmlEngine>
#include <QQmlContext>

#include <Volcano/Graphics/Service.hpp>
#include <Volcano/Input/Service.hpp>
#include <Volcano/Physics/Service.hpp>
#include <Volcano/Sound/Service.hpp>

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Context: public QObject {
    Q_OBJECT

public:
    Context(QObject *parent = nullptr);
    ~Context(void) override;

public:
    virtual Duration elapsed(void) = 0;
    virtual Input::Service *inputService(void) = 0;
    virtual Graphics::Service *graphicsService(void) = 0;
    virtual Physics::Service *physicsService(void) = 0;
    virtual Sound::Service *soundService(void) = 0;

    static Context *fromObject(QObject *object);

protected:
    void attach(QQmlEngine *engine);

signals:
    void graphicsSerivceStarted(void);
    void graphicsServiceStopping(void);
    void physicsServiceStarted(void);
    void physicsServiceStopping(void);
    void soundServiceStarted(void);
    void soundServiceStopping(void);
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CONTEXT_HPP
