//
//
#include <Volcano/Game/Entity.hpp>

VOLCANO_GAME_BEGIN

Entity::Entity(QObject *parent):
    Actor(parent)
{
}

Entity::~Entity(void)
{
}

Transform *Entity::transform(void)
{
    return &m_transform;
}

const ComponentList &Entity::components(void) const
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

void Entity::tick(void)
{
    auto pService = physicsService();
    if (pService != nullptr)
        pService->pushTransform(true);

    for (auto component: m_components)
        component->updateState();

    if (pService != nullptr) {
        m_transform.setData(pService->transform());
        pService->popTransform();
    }
}

void Entity::draw(void)
{
    auto gService = graphicsService();

    gService->pushTransform(true);
    gService->setTransform(m_transform.data());

    for (auto component: m_components)
        component->updateGraphics();

    gService->popTransform();
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
