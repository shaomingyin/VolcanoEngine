//
//
#ifndef VOLCANO_GAME_COMPONENT_H
#define VOLCANO_GAME_COMPONENT_H

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Object.h>

VOLCANO_GAME_BEGIN

class Component: public Object {
    Q_OBJECT

public:
    Component(QObject* parent = nullptr);
    Component(Context& context, QObject* parent = nullptr);

public:
    friend QDataStream& operator<<(QDataStream& s, const Component& v);
    friend QDataStream& operator>>(QDataStream& s, Component& v);
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_COMPONENT_H
