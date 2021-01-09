//
//
#ifndef VOLCANO_SERVER_WORLD_HPP
#define VOLCANO_SERVER_WORLD_HPP

#include <vector>

//#define register
//#include <bullet/btBulletDynamicsCommon.h>
//#undef register

#include <Volcano/Server/Common.hpp>
#include <Volcano/Server/Camera.hpp>
#include <Volcano/Server/Entity.hpp>

VOLCANO_SERVER_BEGIN

using Cameras = std::vector<Camera *>;
using Entities = std::vector<Entity *>;

class World {
public:
    World(void);
    virtual ~World(void);

public:
    bool init(void);
    void shutdown(void);
    void update(float elapsed);

private:
    //btSimpleDynamicsWorld *m_dynamicWorld;
    Cameras m_cameras;
    Entities m_entities;
};

VOLCANO_SERVER_END

#endif // VOLCANO_SERVER_WORLD_HPP
