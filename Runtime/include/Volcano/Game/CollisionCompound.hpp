//
//
#ifndef VOLCANO_GAME_COLLISIONCOMPOUND_HPP
#define VOLCANO_GAME_COLLISIONCOMPOUND_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/CollisionShape.hpp>

VOLCANO_GAME_BEGIN

class CollisionCompound: public CollisionShape {
    Q_OBJECT

public:
    CollisionCompound(QObject *parent = nullptr);
    ~CollisionCompound(void) override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_COLLISIONCOMPOUND_HPP
