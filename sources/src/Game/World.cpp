//
//
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

World::World(QObject *parent):
    QObject(parent),
    m_mainCamera(nullptr),
    m_gravity(0.0f, -9.8f, 0.0f)
{
}

World::~World(void)
{
}

const QString &World::name(void) const
{
    return m_name;
}

void World::setName(const QString &v)
{
    if (m_name != v)
    {
        m_name = v;
        nameChanged(v);
    }
}

Camera *World::mainCamera(void)
{
    return m_mainCamera;
}

void World::setMainCamera(Camera *cam)
{
    if (m_mainCamera != cam)
    {
        m_mainCamera = cam;
        mainCameraChanged(cam);
    }
}

const QVector3D &World::gravity(void) const
{
    return m_gravity;
}

void World::setGravity(const QVector3D &v)
{
    if (m_gravity != v)
    {
        m_gravity = v;
        gravityChanged(v);
    }
}

QQmlListProperty<Entity> World::qmlEntities(void)
{
    return QQmlListProperty<Entity>(this, this,
        &World::qmlAppendEntity,
        &World::qmlEntityCount,
        &World::qmlEntityAt,
        &World::qmlClearEntities,
        &World::qmlReplaceEntity,
        &World::qmlRemoveLastEntity);
}

void World::update(float elapsed)
{
}

void World::buildView(Graphics::View &v)
{
    if (Q_UNLIKELY(m_mainCamera == nullptr))
        return;
}

void World::appendEntity(Entity *ent)
{
    // TODO

    m_entities.append(ent);
}

void World::qmlAppendEntity(QQmlListProperty<Entity> *list, Entity *ent)
{
    reinterpret_cast<World *>(list->data)->appendEntity(ent);
}

int World::entityCount(void) const
{
    return m_entities.count();
}

int World::qmlEntityCount(QQmlListProperty<Entity> *list)
{
    return reinterpret_cast<World *>(list->data)->entityCount();
}

Entity *World::entityAt(int i)
{
    return m_entities.at(i);
}

Entity *World::qmlEntityAt(QQmlListProperty<Entity> *list, int i)
{
    return reinterpret_cast<World *>(list->data)->entityAt(i);
}

void World::clearEntities(void)
{
    // TODO

    m_entities.clear();
}

void World::qmlClearEntities(QQmlListProperty<Entity> *list)
{
    reinterpret_cast<World *>(list->data)->clearEntities();
}

void World::replaceEntity(int i, Entity *ent)
{
    // TODO

    m_entities.replace(i, ent);
}

void World::qmlReplaceEntity(QQmlListProperty<Entity> *list, int i, Entity *ent)
{
    reinterpret_cast<World *>(list->data)->replaceEntity(i, ent);
}

void World::removeLastEntity(void)
{
    // TODO

    m_entities.removeLast();
}

void World::qmlRemoveLastEntity(QQmlListProperty<Entity> *list)
{
    reinterpret_cast<World *>(list->data)->removeLastEntity();
}

VOLCANO_GAME_END
