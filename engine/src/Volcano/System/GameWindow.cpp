//
//
#include <memory>

#include <Volcano/System/GameWindow.hpp>

VOLCANO_SYSTEM_BEGIN

GameWindow::GameWindow(QWindow *parent):
    QQuickView(parent),
    m_gameWorld(nullptr)
{
}

GameWindow::~GameWindow(void)
{
}

Game::World *GameWindow::gameWorld(void)
{
    return m_gameWorld;
}

void GameWindow::setGameWorld(Game::World *p)
{
    if (m_gameWorld != p) {
        m_gameWorld = p;
        emit gameWorldChanged(p);
    }
}

VOLCANO_SYSTEM_END
