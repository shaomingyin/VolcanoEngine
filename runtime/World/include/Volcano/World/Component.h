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

public:
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_COMPONENT_H
