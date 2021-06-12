//
//
#ifndef VOLCANO_CLIENT_WORLDVIEW_HPP
#define VOLCANO_CLIENT_WORLDVIEW_HPP

#include <QQuickFramebufferObject>

#include <Volcano/Game/World.hpp>
#include <Volcano/Client/Common.hpp>

VOLCANO_CLIENT_BEGIN

class WorldView: public QQuickFramebufferObject {
    Q_OBJECT
    Q_PROPERTY(Game::World *world READ world WRITE setWorld NOTIFY worldChanged)

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
    Game::World *m_world;
};

VOLCANO_CLIENT_END

#endif // VOLCANO_CLIENT_WORLDVIEW_HPP
