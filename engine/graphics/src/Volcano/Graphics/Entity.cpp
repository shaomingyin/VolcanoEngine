//
//
#include <QObject>

#include <Volcano/Game/Mesh.hpp>
#include <Volcano/Graphics/Entity.hpp>

VOLCANO_GRAPHICS_BEGIN

Entity::Entity(Game::Entity *gameEntity):
    m_gameEntity(gameEntity)
{
    init();
}

Entity::Entity(Entity &&that):
    m_gameEntity(that.m_gameEntity)
{
    that.release();
    init();
}

Entity::Entity(const Entity &that):
    m_gameEntity(that.m_gameEntity),
    m_meshList(that.m_meshList)
{
    init();
}

Entity::~Entity(void)
{
}

void Entity::synchronize(void)
{
}

void Entity::addToVS(VisibleSet &vs)
{
}

Entity &Entity::operator=(Entity &&that)
{
    m_gameEntity = that.m_gameEntity;
    m_meshList = std::move(that.m_meshList);

    that.release();
    init();

    return (*this);
}

Entity &Entity::operator=(const Entity &that)
{
    if (this != &that) {
        m_gameEntity = that.m_gameEntity;
        m_meshList = that.m_meshList;
        init();
    }

    return (*this);
}

void Entity::init(void)
{
    Q_ASSERT(m_gameEntity != nullptr);

    auto gameComponents = m_gameEntity->components();
    for (auto gameComponent: gameComponents)
        onGameComponentAdded(gameComponent);

    m_gameComponentAddedConnection = QObject::connect(m_gameEntity, &Game::Entity::componentAdded,
        [this] (Game::Component *gameComponent) {
            onGameComponentAdded(gameComponent);
        }
    );

    m_gameComponentRemovedConnection = QObject::connect(m_gameEntity, &Game::Entity::componentRemoved,
        [this] (Game::Component *gameComponent) {
            onGameComponentRemoved(gameComponent);
        }
    );
}

void Entity::release(void)
{
    QObject::disconnect(m_gameComponentAddedConnection);
    QObject::disconnect(m_gameComponentRemovedConnection);

    m_gameEntity = nullptr;
}

void Entity::onGameComponentAdded(Game::Component *gameComponent)
{
    auto gameMesh = qobject_cast<Game::Mesh *>(gameComponent);
    if (gameMesh == nullptr)
        return;

    auto &ref = m_meshList.emplaceBack(gameMesh);
}

void Entity::onGameComponentRemoved(Game::Component *gameComponent)
{
}

VOLCANO_GRAPHICS_END
