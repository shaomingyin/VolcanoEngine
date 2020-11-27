//
//
#ifndef VOLCANO_VM_WORLD_HPP
#define VOLCANO_VM_WORLD_HPP

#include <vector>

#define register
#include <btBulletDynamicsCommon.h>
#undef register

#include <Volcano/Graphics/View.hpp>
#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/Camera.hpp>
#include <Volcano/VM/Entity.hpp>

VOLCANO_VM_BEGIN

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
    void handleEvent(const SDL_Event &evt);
    void applyToView(Graphics::View &view);

private:
    btSimpleDynamicsWorld *m_dynamicWorld;
    Cameras m_cameras;
    Entities m_entities;
};

VOLCANO_VM_END

#endif // VOLCANO_VM_WORLD_HPP
