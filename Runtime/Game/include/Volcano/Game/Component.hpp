//
//
#ifndef VOLCANO_GAME_COMPONENT_HPP
#define VOLCANO_GAME_COMPONENT_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Actor.hpp>

VOLCANO_GAME_BEGIN

class VOLCANO_GAME_API Component: public Actor {
    Q_OBJECT

public:
    Component(QObject* parent = nullptr);
    Component(Context& context, QObject* parent = nullptr);
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_COMPONENT_HPP
