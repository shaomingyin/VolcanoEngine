//
//
#ifndef VOLCANO_GAME_COLLISIONSHAPE_HPP
#define VOLCANO_GAME_COLLISIONSHAPE_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Component.hpp>

VOLCANO_GAME_BEGIN

class CollisionShape: public Component {
    Q_OBJECT

public:
    CollisionShape(QObject *parent = nullptr);
    ~CollisionShape(void) override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_COLLISIONSHAPE_HPP
