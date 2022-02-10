//
//
#ifndef VOLCANO_GAME_MANIFEST_HPP
#define VOLCANO_GAME_MANIFEST_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Object.hpp>
#include <Volcano/Game/World.hpp>
#include <Volcano/Game/Actor.hpp>
#include <Volcano/Game/Camera.hpp>
#include <Volcano/Game/Screen.hpp>

VOLCANO_GAME_BEGIN

class Manifest: public Object {
    Q_OBJECT
    Q_PROPERTY(World *world READ world WRITE setWorld NOTIFY worldChanged)
    Q_PROPERTY(Actor *player READ player WRITE setPlayer NOTIFY playerChanged)
    Q_PROPERTY(Camera *camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(Screen *overlay READ overlay WRITE setOverlay NOTIFY overlayChanged)

public:
    Manifest(QObject *parent = nullptr);
    ~Manifest(void) override;

public:
    World *world(void);
    void setWorld(World *p);
    Actor *player(void);
    void setPlayer(Actor *p);
    Camera *camera(void);
    void setCamera(Camera *p);
    Screen *overlay(void);
    void setOverlay(Screen *p);

protected:
    void timerEvent(QTimerEvent *event) override;
    bool event(QEvent *event) override;
    void tick(void) override;
    void draw(void) override;

signals:
    void worldChanged(World *p);
    void playerChanged(Actor *p);
    void cameraChanged(Camera *p);
    void overlayChanged(Screen *p);
    void test(void);

private:
    World *m_world;
    Actor *m_player;
    Camera *m_camera;
    Screen *m_overlay;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_MANIFEST_HPP
