//
//
#ifndef VOLCANO_GAME_ENTITY_HPP
#define VOLCANO_GAME_ENTITY_HPP

#include <QList>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Component.hpp>
#include <Volcano/Game/Actor.hpp>

VOLCANO_GAME_BEGIN

class Entity: public Actor {
    Q_OBJECT

public:
    Entity(QObject *parent = nullptr);
    ~Entity(void) override;

public:
    void appendComponent(Component *p);
    Component *componentAt(qsizetype index);
    const Component *componentAt(qsizetype index) const;
    void clearComponents(void);
    qsizetype componentCount(void) const;
    void removeLastComponent(void);
    void replaceComponent(qsizetype index, Component *p);
    const QList<Component *> &components(void) const;

private:
    QList<Component *> m_componentList;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ENTITY_HPP
