//
//
#ifndef VOLCANO_VM_SCENE_WORLD_HPP
#define VOLCANO_VM_SCENE_WORLD_HPP

#include <vector>

#include <Volcano/VM/Scene/Common.hpp>
#include <Volcano/VM/Scene/Camera.hpp>
#include <Volcano/VM/Scene/Entity.hpp>

VOLCANO_VM_SCENE_BEGIN

//typedef std::vector<entity *> entity_ptr_array_t;

class World { RTTR_ENABLE()
public:
    World(void);
    virtual ~World(void);

public:
    bool init(void);
    void shutdown(void);
    void update(float elapsed);
    void handleEvent(const SDL_Event &evt);
    void test(int i);

private:
    //entity_ptr_array_t m_entities;
};

VOLCANO_VM_SCENE_END

#endif // VOLCANO_VM_SCENE_WORLD_HPP
