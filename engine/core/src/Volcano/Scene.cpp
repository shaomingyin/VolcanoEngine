//
//
#include <Volcano/Scene.hpp>

VOLCANO_BEGIN

Scene::Scene(QObject *parent):
    Object(parent),
    m_btWorld(nullptr),
    m_gravity(0.0f, -9.8f, 0.0f)
{
}

Scene::~Scene(void)
{
    if (m_btWorld != nullptr) {
        // TODO destroy bt world.
    }
}

bool Scene::isDynamic(void) const
{
    return (m_btWorld != nullptr);
}

void Scene::setDynamic(bool v)
{
    if (v == isDynamic())
        return;

    if (v) {
        // TODO create bt world.
        // TODO apply m_gravity to new this bt world.
    } else {
        // TODO destroy bt world.
    }

    dynamicChanged(v);
}

const QVector3D &Scene::gravity(void) const
{
    return m_gravity;
}

void Scene::setGravity(const QVector3D &v)
{
    if (m_gravity == v)
        return;

    m_gravity = v;

    if (m_btWorld != nullptr)
        m_btWorld->setGravity(btVector3(v[0], v[1], v[2]));

    gravityChanged(v);
}

QQmlListProperty<Object> Scene::objects(void)
{
    return { this, this,
        &Scene::appendObject,
        &Scene::objectCount,
        &Scene::objectAt,
        &Scene::clearObjects,
        &Scene::replaceObject,
        &Scene::removeLastObject };
}

void Scene::appendObject(QQmlListProperty<Object> *list, Object *object)
{
    auto scene = reinterpret_cast<Scene *>(list->data);
    scene->m_objects.append(object);
}

qsizetype Scene::objectCount(QQmlListProperty<Object> *list)
{
    auto scene = reinterpret_cast<Scene *>(list->data);
    return scene->m_objects.count();
}

Object *Scene::objectAt(QQmlListProperty<Object> *list, qsizetype i)
{
    auto scene = reinterpret_cast<Scene *>(list->data);
    return scene->m_objects.at(i);
}

void Scene::clearObjects(QQmlListProperty<Object> *list)
{
    auto scene = reinterpret_cast<Scene *>(list->data);
    scene->m_objects.clear();
}

void Scene::replaceObject(QQmlListProperty<Object> *list, qsizetype i, Object *object)
{
    auto scene = reinterpret_cast<Scene *>(list->data);
    scene->m_objects.replace(i, object);
}

void Scene::removeLastObject(QQmlListProperty<Object> *list)
{
    auto scene = reinterpret_cast<Scene *>(list->data);
    scene->m_objects.removeLast();
}

VOLCANO_END
