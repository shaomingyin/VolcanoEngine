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
        disconnect(m_gameWorld, &Game::WorldBase::objectAdded, this, &World::onGameObjectAdded);
        disconnect(m_gameWorld, &Game::WorldBase::objectRemoved, this, &World::onGameObjectRemoved);
        reset();
    }

    m_gameWorld = p;

    if (m_gameWorld != nullptr) {
        connect(m_gameWorld, &Game::WorldBase::objectAdded, this, &World::onGameObjectAdded);
        connect(m_gameWorld, &Game::WorldBase::objectRemoved, this, &World::onGameObjectRemoved);
        for (auto gameObject: m_gameWorld->objects())
            onGameObjectAdded(gameObject);
    }

    emit gameWorldChanged(p);
}

void World::buildView(View *view, const Camera &cam)
{
    Q_ASSERT(view != nullptr);

    // TODO use bsp/octree here?
    // TODO add lights...

    for (const auto &ent: m_entityList) {
        if (ent->isValid()) {
            // check bbox?
            ent->buildView(view, cam);
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
    // TODO
}

void World::onGameDirectionalLightAdded(Game::DirectionalLight *p)
{
    m_gameDirectionalLightList.append(p);
}

void World::onGameDirectionalLightRemoved(Game::DirectionalLight *p)
{

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
    auto ent = std::make_unique<Entity>(p, this);
    if (ent)
        m_entityList.append(ent.release());
}

void World::onGameEntityRemoved(Game::Entity *p)
{
    for (auto it = m_entityList.begin(); it != m_entityList.end(); ++it) {
        if ((*it)->gameEntity() == p) {
            m_entityList.erase(it);
            delete *it;
            break;
        }
    }
}

void World::onGameObjectAdded(Game::Object *p)
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

void World::onGameObjectRemoved(Game::Object *p)
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
