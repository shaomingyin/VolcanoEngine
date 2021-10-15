//
//
#ifndef VOLCANO_SYSTEM_GAMEWINDOW_HPP
#define VOLCANO_SYSTEM_GAMEWINDOW_HPP

#include <QQuickView>

#include <Volcano/Game/World.hpp>
#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class GameWindow: public QQuickView {
    Q_OBJECT
    Q_PROPERTY(Game::World *gameWorld)

public:
    GameWindow(QWindow *parent = nullptr);
    ~GameWindow(void) override;

public:
    Game::World *gameWorld(void);
    void setGameWorld(Game::World *p);

signals:
    void gameWorldChanged(Game::World *p);

private:
    Game::World *m_gameWorld;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_GAMEWINDOW_HPP
