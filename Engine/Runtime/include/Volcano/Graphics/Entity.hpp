//
//
#ifndef VOLCANO_GRAPHICS_ENTITY_HPP
#define VOLCANO_GRAPHICS_ENTITY_HPP

#include <QList>
#include <QObject>

#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Component.hpp>
#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Graphics/View.hpp>
#include <Volcano/Graphics/Visual.hpp>

VOLCANO_GRAPHICS_BEGIN

class Entity: public QObject {
    Q_OBJECT

public:
    Entity(Game::Entity *p, QObject *parent = nullptr);
    ~Entity(void) override;

public:
    Game::Entity *gameEntity(void);
    bool isValid(void) const;
    void buildView(View *view, const Camera &cam) const;

private:
    void onShapeChanged(Game::Visual *gameVisual, Shape *shape);
    void onGameVisualAdded(Game::Visual *v);
    void onGameVisualRemoved(Game::Visual *v);

private slots:
    void onGameComponentAdded(Game::Component *comp);
    void onGameComponentRemoved(Game::Component *comp);

private:
    using VisualList = QList<Visual *>;

    Game::Entity *m_gameEntity;
    VisualList m_visualList;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_ENTITY_HPP
