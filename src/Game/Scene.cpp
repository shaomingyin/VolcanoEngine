//
//
#include <Volcano/Game/Scene.hpp>

VOLCANO_GAME_BEGIN

Scene::Scene(QObject *parent):
    QObject(parent),
    m_gravity(0.0f, -9.8f, 0.0f),
    m_btWorld(nullptr)
{
}

Scene::~Scene(void)
{
    if (m_btWorld != nullptr)
        delete m_btWorld;
}

bool Scene::init(void)
{
    Q_ASSERT(m_btWorld == nullptr);

    // TODO init bullet physics.

    return true;
}

void Scene::setGravity(const QVector3D &r)
{
    if (m_gravity == r)
        return;

    m_gravity = r;

    if (m_btWorld != nullptr)
        m_btWorld->setGravity(btVector3(r.x(), r.y(), r.z()));

    gravityChanged(r);
}

QQmlListProperty<Light> Scene::lights(void)
{
    return QQmlListProperty<Light>(this, this,
        &Scene::appendLight,
        &Scene::lightCount,
        &Scene::lightAt,
        &Scene::clearLight,
        &Scene::replaceLight,
        &Scene::removeLastLight);
}

QQmlListProperty<Entity> Scene::entities(void)
{
    return QQmlListProperty<Entity>(this, this,
        &Scene::appendEntity,
        &Scene::entityCount,
        &Scene::entityAt,
        &Scene::clearEntity,
        &Scene::replaceEntity,
        &Scene::removeLastEntity);
}

void Scene::update(float elapsed)
{
    if (m_btWorld != nullptr)
    {
        m_btWorld->stepSimulation(elapsed);

        // TODO
    }
}

void Scene::appendLight(QQmlListProperty<Volcano::Game::Light> *p, Light *n)
{
    reinterpret_cast<Scene *>(p->data)->m_lightList.append(n);
}

int Scene::lightCount(QQmlListProperty<Volcano::Game::Light> *p)
{
    return reinterpret_cast<Scene *>(p->data)->m_lightList.count();
}

Light *Scene::lightAt(QQmlListProperty<Volcano::Game::Light> *p, int i)
{
    return reinterpret_cast<Scene *>(p->data)->m_lightList.at(i);
}

void Scene::clearLight(QQmlListProperty<Volcano::Game::Light> *p)
{
    reinterpret_cast<Scene *>(p->data)->m_lightList.clear();
}

void Scene::replaceLight(QQmlListProperty<Volcano::Game::Light> *p, int i, Light *n)
{
    reinterpret_cast<Scene *>(p->data)->m_lightList.replace(i, n);
}

void Scene::removeLastLight(QQmlListProperty<Volcano::Game::Light> *p)
{
    reinterpret_cast<Scene *>(p->data)->m_lightList.removeLast();
}

void Scene::appendEntity(QQmlListProperty<Volcano::Game::Entity> *p, Entity *n)
{
    reinterpret_cast<Scene *>(p->data)->m_entityList.append(n);
}

int Scene::entityCount(QQmlListProperty<Volcano::Game::Entity> *p)
{
    return reinterpret_cast<Scene *>(p->data)->m_entityList.count();
}

Entity *Scene::entityAt(QQmlListProperty<Volcano::Game::Entity> *p, int i)
{
    return reinterpret_cast<Scene *>(p->data)->m_entityList.at(i);
}

void Scene::clearEntity(QQmlListProperty<Volcano::Game::Entity> *p)
{
    reinterpret_cast<Scene *>(p->data)->m_entityList.clear();
}

void Scene::replaceEntity(QQmlListProperty<Volcano::Game::Entity> *p, int i, Entity *n)
{
    reinterpret_cast<Scene *>(p->data)->m_entityList.replace(i, n);
}

void Scene::removeLastEntity(QQmlListProperty<Volcano::Game::Entity> *p)
{
    reinterpret_cast<Scene *>(p->data)->m_entityList.removeLast();
}

VOLCANO_GAME_END
