//
//
#include <Volcano/Game/Entity.hpp>

VOLCANO_GAME_BEGIN

Entity::Entity(QObject *parent):
    Object(parent)
{
}

Entity::~Entity(void)
{
}

void Entity::update(float elapsed)
{
    for (auto component: m_components)
        component->update(elapsed);
}

const QVector3D &Entity::position(void) const
{
    return m_position;
}

void Entity::setPosition(const QVector3D &v)
{
    if (!qFuzzyCompare(m_position, v)) {
        m_position = v;
        emit positionChanged(v);
    }
}

const QVector3D &Entity::scale(void) const
{
    return m_scale;
}

void Entity::setScale(const QVector3D &v)
{
    if (!qFuzzyCompare(m_scale, v)) {
        m_scale = v;
        emit scaleChanged(v);
    }
}

const QQuaternion &Entity::rotation(void)
{
    return m_rotation;
}

void Entity::setRotation(const QQuaternion &v)
{
    if (!qFuzzyCompare(m_rotation, v)) {
        m_rotation = v;
        emit rotationChanged(v);
    }
}

const QList<Component *> &Entity::components(void) const
{
    return m_components;
}

QQmlListProperty<Component> Entity::qmlComponents(void)
{
    return { this, this,
        &Entity::qmlAppendComponent,
        &Entity::qmlComponentCount,
        &Entity::qmlComponentAt,
        &Entity::qmlClearComponents,
        &Entity::qmlReplaceComponent,
        &Entity::qmlRemoveLastComponent };
}

void Entity::appendComponent(Component *component)
{
    m_components.append(component);
    handleComponentAdded(component);
}

qsizetype Entity::componentCount(void) const
{
    return m_components.count();
}

Component *Entity::componentAt(qsizetype i)
{
    if (0 <= i && i < m_components.size())
        return m_components.at(i);
    return nullptr;
}

const Component *Entity::componentAt(qsizetype i) const
{
    if (0 <= i && i < m_components.size())
        return m_components.at(i);
    return nullptr;
}

void Entity::clearComponents(void)
{
    auto components = std::move(m_components);
    for (auto component: components)
        handleComponentRemoved(component);
}

void Entity::replaceComponent(qsizetype i, Component *component)
{
    if (0 <= i && i < m_components.size()) {
        auto oldComponent = m_components.at(i);
        m_components.replace(i, component);
        handleComponentRemoved(oldComponent);
        handleComponentAdded(component);
    }
}

void Entity::removeLastComponent(void)
{
    if (!m_components.isEmpty()) {
        auto lastComponent = m_components.last();
        m_components.removeLast();
        handleComponentRemoved(lastComponent);
    }
}

void Entity::handleComponentAdded(Component *component)
{
    component->setParent(this);
    emit componentAdded(component);
}

void Entity::handleComponentRemoved(Component *component)
{
    component->setParent(nullptr);
    emit componentRemoved(component);
}

void Entity::qmlAppendComponent(QQmlListProperty<Component> *list, Component *component)
{
    reinterpret_cast<Entity *>(list->data)->appendComponent(component);
}

qsizetype Entity::qmlComponentCount(QQmlListProperty<Component> *list)
{
    return reinterpret_cast<Entity *>(list->data)->componentCount();
}

Component *Entity::qmlComponentAt(QQmlListProperty<Component> *list, qsizetype i)
{
    return reinterpret_cast<Entity *>(list->data)->componentAt(i);
}

void Entity::qmlClearComponents(QQmlListProperty<Component> *list)
{
    reinterpret_cast<Entity *>(list->data)->clearComponents();
}

void Entity::qmlReplaceComponent(QQmlListProperty<Component> *list, qsizetype i, Component *component)
{
    reinterpret_cast<Entity *>(list->data)->replaceComponent(i, component);
}

void Entity::qmlRemoveLastComponent(QQmlListProperty<Component> *list)
{
    reinterpret_cast<Entity *>(list->data)->removeLastComponent();
}

VOLCANO_GAME_END
