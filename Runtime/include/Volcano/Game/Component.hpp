//
//
#ifndef VOLCANO_GAME_COMPONENT_HPP
#define VOLCANO_GAME_COMPONENT_HPP

#include <QList>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Context.hpp>
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

class Component: public Object {
    Q_OBJECT

public:
    Component(QObject *parent = nullptr);
    Component(Context *context, QObject *parent = nullptr);
    ~Component(void) override;
};

using ComponentList = QList<Component *>;

VOLCANO_GAME_END

#endif // VOLCANO_GAME_COMPONENT_HPP
