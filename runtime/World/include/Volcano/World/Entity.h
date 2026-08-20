//
//
#ifndef VOLCANO_WORLD_ENTITY_H
#define VOLCANO_WORLD_ENTITY_H

#include <QList>
#include <QMatrix4x4>
#include <QQmlListProperty>
#include <QObject>

#include <Volcano/World/Common.h>
#include <Volcano/World/Component.h>
#include <Volcano/World/Transform.h>

VOLCANO_WORLD_BEGIN

using ComponentList = QList<Component*>;

class World;

class Entity: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(Transform* transform READ transform)
    Q_PROPERTY(QQmlListProperty<Component> components READ qmlComponents)
    Q_CLASSINFO("DefaultProperty", "components")

public:
    Entity(QObject* parent = nullptr);
    ~Entity() override;

public:
    void init(entt::registry& registry);
    entt::handle handle() noexcept;
    Transform* transform() noexcept;
    bool isEnabled() const noexcept;
    void setEnabled(bool v) noexcept;
    const ComponentList& components() const noexcept;
    void appendComponent(Component* p);
    Component* componentAt(qsizetype i);
    void clearComponents();
    qsizetype componentCount();
    void removeLastComponent();
    void replaceComponent(qsizetype i, Component* p);
    QQmlListProperty<Component> qmlComponents();

signals:
    void enabledChanged(bool v);
    void componentAdded(Component* p);
    void componentRemoved(Component* p);

private:
    bool enabled_;
    Transform transform_;
    entt::handle handle_;
    ComponentList components_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_ENTITY_H
