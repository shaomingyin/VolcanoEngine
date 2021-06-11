//
//
#ifndef VOLCANO_UI_WORLDVIEW_HPP
#define VOLCANO_UI_WORLDVIEW_HPP

#include <QQuickFramebufferObject>

#include <Volcano/Game/World.hpp>
#include <Volcano/UI/Common.hpp>

VOLCANO_UI_BEGIN

class WorldView: public QQuickFramebufferObject {
    Q_OBJECT
    Q_PROPERTY(Game::World *world GET world SET setWorkd NOTIFY worldChanged)

public:
    WorldView(QQuickItem *parent = nullptr);
    ~WorldView(void) override;

public:
    Renderer *createRenderer(void) const override;
    Game::World *world(void);
    void setWorld(Game::World *world);

signals:
    void worldChanged(Game::World *world);

private:
    mutable Renderer *m_renderer;
    Game::World *m_gameWorld;
};

VOLCANO_UI_END

#endif // VOLCANO_UI_WORLDVIEW_HPP
