//
//
#ifndef VOLCANO_GAME_SCENE_HPP
#define VOLCANO_GAME_SCENE_HPP

#include <list>

//#define register
//#include <bullet/btBulletDynamicsCommon.h>
//#undef register

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Camera.hpp>
#include <Volcano/Game/Entity.hpp>

VOLCANO_GAME_BEGIN

using EntityList = std::list<Entity *>;

class Scene {
public:
    Scene(void);
    virtual ~Scene(void);

public:
    bool init(void);
    void shutdown(void);
    void update(float elapsed);

private:
    //btSimpleDynamicsWorld *m_dynamicWorld;
    EntityList m_entityList;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SCENE_HPP
