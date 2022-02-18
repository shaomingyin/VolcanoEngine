//
//
#ifndef VOLCANO_GAME_OBJECT_HPP
#define VOLCANO_GAME_OBJECT_HPP

#include <QObject>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Context.hpp>

VOLCANO_GAME_BEGIN

class Object: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)

public:
    Object(QObject *parent = nullptr);
    Object(Context *context, QObject *parent = nullptr);
    ~Object(void) override;

public:
    bool isEnabled(void) const;
    void setEnabled(bool v);
    void updateState(void);
    void updateGraphics(void);

signals:
    void enabledChanged(bool v);

protected:
    Context *context(void);
    Duration elapsed(void);
    Input::Service *inputService(void);
    Graphics::Service *graphicsService(void);
    Physics::Service *physicsService(void);
    Sound::Service *soundService(void);
    virtual void tick(void);
    virtual void draw(void);

private:
    bool m_isEnabled;
    Context *m_context;
    Input::Service *m_inputService;
    Graphics::Service *m_graphicsService;
    Physics::Service *m_physicsService;
    Sound::Service *m_soundService;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_OBJECT_HPP
