//
//
#ifndef VOLCANO_GAME_DYNAMICWORLD_HPP
#define VOLCANO_GAME_DYNAMICWORLD_HPP

#include <list>
#include <string>
#include <memory>

#include <bullet/btBulletDynamicsCommon.h>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

class DynamicWorld: public World {
public:
    DynamicWorld(void);
    virtual ~DynamicWorld(void);

public:
    void update(float elapsed) override;

private:
    btDynamicsWorld *m_btWorld;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_DYNAMICWORLD_HPP
