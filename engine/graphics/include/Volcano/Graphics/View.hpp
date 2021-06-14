//
//
#ifndef VOLCANO_CLIENT_VIEW_HPP
#define VOLCANO_CLIENT_VIEW_HPP

#include <QQuickFramebufferObject>

#include <Volcano/World.hpp>
#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class View: public QQuickFramebufferObject {
    Q_OBJECT
    Q_PROPERTY(World *world READ world WRITE setWorld NOTIFY worldChanged)

public:
    View(QQuickItem *parent = nullptr);
    ~View(void) override;

public:
    Renderer *createRenderer(void) const override;
    World *world(void);
    void setWorld(World *world);

signals:
    void worldChanged(World *world);

private:
    mutable Renderer *m_renderer;
    World *m_world;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VIEW_HPP
