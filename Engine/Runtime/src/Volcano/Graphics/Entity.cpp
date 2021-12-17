//
//
#include <Volcano/Graphics/Entity.hpp>

VOLCANO_GRAPHICS_BEGIN

Entity::Entity(Game::Entity *p, QObject *parent):
    QObject(parent),
    m_gameEntity(p)
{
    Q_ASSERT(p != nullptr);

    connect(p, &Game::Entity::componentAdded, this, &Entity::onGameComponentAdded);
    connect(p, &Game::Entity::componentRemoved, this, &Entity::onGameComponentRemoved);

    for (auto comp: p->components())
        onGameComponentAdded(comp);
}

Entity::~Entity(void)
{
}

Game::Entity *Entity::gameEntity(void)
{
    return m_gameEntity;
}

bool Entity::isValid(void) const
{
    Q_ASSERT(m_gameEntity != nullptr);

    if (!m_gameEntity->isEnabled())
        return false;

    for (auto visual: m_visualList) {
        if (!visual->isValid())
            return false;
    }

    return true;
}

void Entity::buildView(View *view, const Camera &cam) const
{
    Q_ASSERT(view != nullptr);

    if (!m_gameEntity->isEnabled())
        return;

    view->translateTo(m_gameEntity->position());
    view->scaleTo(m_gameEntity->scale());
    view->rotateTo(m_gameEntity->rotation());

    for (const auto visual: m_visualList) {
        view->push(true);
        visual->buildView(view, cam);
        view->pop();
    }
}

void Entity::onGameVisualAdded(Game::Visual *p)
{
    m_visualList.append(new Visual(p));
}

void Entity::onGameVisualRemoved(Game::Visual *p)
{
    m_visualList.removeIf([p](Visual *v) { return (v->gameVisual() == p); });
}

void Entity::onGameComponentAdded(Game::Component *p)
{
    auto gameVisual = qobject_cast<Game::Visual *>(p);
    if (gameVisual != nullptr) {
        onGameVisualAdded(gameVisual);
        return;
    }
}

void Entity::onGameComponentRemoved(Game::Component *p)
{
    auto gameVisual = qobject_cast<Game::Visual *>(p);
    if (gameVisual != nullptr) {
        onGameVisualRemoved(gameVisual);
        return;
    }
}

VOLCANO_GRAPHICS_END
