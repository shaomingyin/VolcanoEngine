//
//
#ifndef VOLCANO_GAME_ENTITY_HPP
#define VOLCANO_GAME_ENTITY_HPP

#include <QList>
#include <QQmlListProperty>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Object.hpp>
#include <Volcano/Game/Component.hpp>

VOLCANO_GAME_BEGIN

class Entity: public Object {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Component> scenes READ components)
    Q_CLASSINFO("DefaultProperty", "components")

public:
    Entity(QObject *parent = nullptr);
    ~Entity(void) override;

public:
    QQmlListProperty<Component> components(void);

private:
    static void appendComponent(QQmlListProperty<Component> *list, Component *scene);
    static qsizetype componentCount(QQmlListProperty<Component> *list);
    static Component *componentAt(QQmlListProperty<Component> *list, qsizetype index);
    static void clearComponents(QQmlListProperty<Component> *list);
    static void replaceComponent(QQmlListProperty<Component> *list, qsizetype index, Component *scene);
    static void removeLastComponent(QQmlListProperty<Component> *list);

private:
    QList<Component *> m_components;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ENTITY_HPP
