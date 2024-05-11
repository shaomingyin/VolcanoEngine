//
//
#ifndef VOLCANO_GAME_ENTITY_HPP
#define VOLCANO_GAME_ENTITY_HPP

#include <QList>
#include <QQmlListProperty>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Component.hpp>
#include <Volcano/Game/Actor.hpp>

VOLCANO_GAME_BEGIN

class VOLCANO_GAME_API Entity: public Actor {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Component> components READ qmlComponentList)
    Q_CLASSINFO("DefaultProperty", "components")

public:
    Entity(QObject* parent = nullptr);
    Entity(Context& context, QObject* parent = nullptr);

public:
    const QList<Component*> &componentList(void) const;
    void appendComponent(Component* p);
    qsizetype componentCount(void) const;
    Component* componentAt(qsizetype index);
    const Component* componentAt(qsizetype index) const;
    void clearComponents(void);
    void replaceComponent(qsizetype index, Component* p);
    void removeLastComponent(void);

public:
    QQmlListProperty<Component> qmlComponentList() {
        return { this, this,
            [](QQmlListProperty<Component>* property, Component* p) { reinterpret_cast<Entity*>(property->data)->appendComponent(p); },
            [](QQmlListProperty<Component>* property) { return reinterpret_cast<Entity*>(property->data)->componentCount(); },
            [](QQmlListProperty<Component>* property, qsizetype index) { return reinterpret_cast<Entity*>(property->data)->componentAt(index); },
            [](QQmlListProperty<Component>* property) { reinterpret_cast<Entity*>(property->data)->clearComponents(); },
            [](QQmlListProperty<Component>* property, qsizetype index, Component* p) { reinterpret_cast<Entity*>(property->data)->replaceComponent(index, p); },
            [](QQmlListProperty<Component>* property) { reinterpret_cast<Entity*>(property->data)->removeLastComponent(); }
        };
    }

signals:
    void componentAdded(Component* p);
    void componentRemoved(Component* p);

protected:
    void updateView() const override;
    void updateState(Duration elapsed) override;

private:
    QList<Component*> componentList_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ENTITY_HPP
