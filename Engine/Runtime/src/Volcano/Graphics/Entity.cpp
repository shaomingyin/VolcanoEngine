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

    // TODO return false if no visible component(s).
    return m_gameEntity->isEnabled();
}

void Entity::buildView(View *view, const Camera &cam) const
{
    Q_ASSERT(view != nullptr);

    view->push();
    view->translate(m_gameEntity->position());
    view->scale(m_gameEntity->scale());
    view->rotate(m_gameEntity->rotation());
    // TODO add mesh...
    view->pop();
}

void Entity::onGameMaterialAdded(Game::Material *p)
{
}

void Entity::onGameMaterialRemoved(Game::Material *p)
{
    auto url = p->source();
}

void Entity::onGameMeshAdded(Game::Mesh *p)
{
}

void Entity::onGameMeshRemoved(Game::Mesh *p)
{
}

void Entity::onGameComponentAdded(Game::Component *p)
{
    auto gameMaterial = qobject_cast<Game::Material *>(p);
    if (gameMaterial != nullptr) {
        onGameMaterialAdded(gameMaterial);
        return;
    }

    auto gameMesh = qobject_cast<Game::Mesh *>(p);
    if (gameMesh != nullptr) {
        onGameMeshAdded(gameMesh);
        return;
    }
}

void Entity::onGameComponentRemoved(Game::Component *p)
{
    auto gameMaterial = qobject_cast<Game::Material *>(p);
    if (gameMaterial != nullptr) {
        onGameMaterialRemoved(gameMaterial);
        return;
    }

    auto gameMesh = qobject_cast<Game::Mesh *>(p);
    if (gameMesh != nullptr) {
        onGameMeshRemoved(gameMesh);
        return;
    }
}

VOLCANO_GRAPHICS_END
