//
//
#ifndef VOLCANO_GAME_ENTITY_HPP
#define VOLCANO_GAME_ENTITY_HPP

#include <QQmlListProperty>

#include <Volcano/Transform.hpp>
#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Context.hpp>
#include <Volcano/Game/Component.hpp>
#include <Volcano/Game/Actor.hpp>

VOLCANO_GAME_BEGIN

class Entity: public Actor {
    Q_OBJECT
    Q_PROPERTY(Transform *transform READ transform)
    Q_PROPERTY(QQmlListProperty<Component> components READ qmlComponents)
    Q_CLASSINFO("DefaultProperty", "components")

public:
    Entity(QObject *parent = nullptr);
    Entity(Context *context, QObject *parent = nullptr);
    ~Entity(void) override;

public:
    Transform *transform(void);
    const ComponentList &components(void) const;
    QQmlListProperty<Component> qmlComponents(void);
    void appendComponent(Component *component);
    qsizetype componentCount(void) const;
    Component *componentAt(qsizetype index);
    const Component *componentAt(qsizetype index) const;
    void clearComponents(void);
    void replaceComponent(qsizetype index, Component *component);
    void removeLastComponent(void);

signals:
    void componentAdded(Component *component);
    void componentRemoved(Component *component);

protected:
    void tick(void) override;
    void draw(void) override;

private:
    void handleComponentAdded(Component *component);
    void handleComponentRemoved(Component *component);
    static void qmlAppendComponent(QQmlListProperty<Component> *list, Component *component);
    static qsizetype qmlComponentCount(QQmlListProperty<Component> *list);
    static Component *qmlComponentAt(QQmlListProperty<Component> *list, qsizetype index);
    static void qmlClearComponents(QQmlListProperty<Component> *list);
    static void qmlReplaceComponent(QQmlListProperty<Component> *list, qsizetype index, Component *component);
    static void qmlRemoveLastComponent(QQmlListProperty<Component> *list);

private:
    Transform m_transform;
    ComponentList m_components;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ENTITY_HPP
