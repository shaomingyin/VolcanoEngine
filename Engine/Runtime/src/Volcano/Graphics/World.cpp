//
//
#include <memory>

#include <Volcano/Graphics/World.hpp>

VOLCANO_GRAPHICS_BEGIN

World::World(QObject *parent):
    QObject(parent)
{
}

World::~World(void)
{
}

void World::reset(void)
{
    // TODO

    m_entityList.clear();
}

Game::WorldBase *World::gameWorld(void)
{
    return m_gameWorld;
}

void World::setGameWorld(Game::WorldBase *p)
{
    if (m_gameWorld == p)
        return;

    if (m_gameWorld != nullptr) {
        disconnect(m_gameWorld, &Game::WorldBase::actorAdded, this, &World::onGameActorAdded);
        disconnect(m_gameWorld, &Game::WorldBase::actorRemoved, this, &World::onGameActorRemoved);
        reset();
    }

    m_gameWorld = p;

    if (m_gameWorld != nullptr) {
        connect(m_gameWorld, &Game::WorldBase::actorAdded, this, &World::onGameActorAdded);
        connect(m_gameWorld, &Game::WorldBase::actorRemoved, this, &World::onGameActorRemoved);
        for (auto gameActor: m_gameWorld->actors())
            onGameActorAdded(gameActor);
    }

    emit gameWorldChanged(p);
}

void World::buildView(View *view, const Camera &cam)
{
    Q_ASSERT(view != nullptr);

    // TODO add lights...

    for (const auto &ent: m_entityList) {
        if (ent->isValid()) {
            // check bbox?
            view->push(true);
            ent->buildView(view, cam);
            view->pop();
        }
    }
}

void World::onGameLightAdded(Game::Light *p)
{
    auto gameSpotLight = qobject_cast<Game::SpotLight *>(p);
    if (gameSpotLight != nullptr) {
        onGameSpotLightAdded(gameSpotLight);
        return;
    }

    auto gameDirectionalLight = qobject_cast<Game::DirectionalLight *>(p);
    if (gameDirectionalLight != nullptr) {
        onGameDirectionalLightAdded(gameDirectionalLight);
        return;
    }

    auto gamePointLight = qobject_cast<Game::PointLight *>(p);
    if (gamePointLight != nullptr) {
        onGamePointLightAdded(gamePointLight);
        return;
    }
}

void World::onGameLightRemoved(Game::Light *p)
{
    auto gameSpotLight = qobject_cast<Game::SpotLight *>(p);
    if (gameSpotLight != nullptr) {
        onGameSpotLightRemoved(gameSpotLight);
        return;
    }

    auto gameDirectionalLight = qobject_cast<Game::DirectionalLight *>(p);
    if (gameDirectionalLight != nullptr) {
        onGameDirectionalLightRemoved(gameDirectionalLight);
        return;
    }

    auto gamePointLight = qobject_cast<Game::PointLight *>(p);
    if (gamePointLight != nullptr) {
        onGamePointLightRemoved(gamePointLight);
        return;
    }
}

void World::onGameDirectionalLightAdded(Game::DirectionalLight *p)
{
    m_gameDirectionalLightList.append(p);
}

void World::onGameDirectionalLightRemoved(Game::DirectionalLight *p)
{
    m_gameDirectionalLightList.removeAll(p);
}

void World::onGamePointLightAdded(Game::PointLight *p)
{
    m_gamePointLightList.append(p);
}

void World::onGamePointLightRemoved(Game::PointLight *p)
{
    m_gamePointLightList.removeAll(p);
}

void World::onGameSpotLightAdded(Game::SpotLight *p)
{
    m_gameSpotLightList.append(p);
}

void World::onGameSpotLightRemoved(Game::SpotLight *p)
{
    m_gameSpotLightList.removeAll(p);
}

void World::onGameDirectionalLightEnabledChanged(Game::DirectionalLight *p, bool isEnabled)
{
    // TODO
}

void World::onGameEntityAdded(Game::Entity *p)
{
    m_entityList.append(new Entity(p));
}

void World::onGameEntityRemoved(Game::Entity *p)
{
    m_entityList.removeIf([p](Entity *ent) { return (ent->gameEntity() == p); });
}

void World::onGameActorAdded(Game::Actor *p)
{
    auto gameLight = qobject_cast<Game::Light *>(p);
    if (gameLight != nullptr) {
        onGameLightAdded(gameLight);
        return;
    }

    auto gameEntity = qobject_cast<Game::Entity *>(p);
    if (gameEntity != nullptr) {
        onGameEntityAdded(gameEntity);
        return;
    }
}

void World::onGameActorRemoved(Game::Actor *p)
{
    auto gameLight = qobject_cast<Game::Light *>(p);
    if (gameLight != nullptr) {
        onGameLightRemoved(gameLight);
        return;
    }

    auto gameEntity = qobject_cast<Game::Entity *>(p);
    if (gameEntity != nullptr) {
        onGameEntityRemoved(gameEntity);
        return;
    }
}

VOLCANO_GRAPHICS_END
