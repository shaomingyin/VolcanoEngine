//
//
#ifndef VOLCANO_GAME_ACTOR_HPP
#define VOLCANO_GAME_ACTOR_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

class Actor: public Object {
    Q_OBJECT

public:
    Actor(QObject *parent = nullptr);
    ~Actor(void) override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ACTOR_HPP
