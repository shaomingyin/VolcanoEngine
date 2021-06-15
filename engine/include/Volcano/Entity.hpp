//
//
#ifndef VOLCANO_ENTITY_HPP
#define VOLCANO_ENTITY_HPP

#include <QList>
#include <QQmlListProperty>

#include <Volcano/Common.hpp>
#include <Volcano/Object.hpp>
#include <Volcano/Component.hpp>

VOLCANO_BEGIN

class Entity: public Object {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Component> components READ qmlComponents)
    Q_CLASSINFO("DefaultProperty", "components")

public:
    Entity(QObject *parent = nullptr);
    ~Entity(void) override;

public:
    const QList<Component *> &components(void) const;
    QQmlListProperty<Component> qmlComponents(void);

signals:
    void componentAdded(Component *component);
    void componentRemoved(Component *component);

private:
    static void appendComponent(QQmlListProperty<Component> *list, Component *component);
    static qsizetype componentCount(QQmlListProperty<Component> *list);
    static Component *componentAt(QQmlListProperty<Component> *list, qsizetype index);
    static void clearComponents(QQmlListProperty<Component> *list);
    static void replaceComponent(QQmlListProperty<Component> *list, qsizetype index, Component *component);
    static void removeLastComponent(QQmlListProperty<Component> *list);

private:
    QList<Component *> m_components;
};

VOLCANO_END

#endif // VOLCANO_ENTITY_HPP
