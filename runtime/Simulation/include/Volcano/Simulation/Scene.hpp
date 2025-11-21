//
//
#ifndef VOLCANO_SIMULATION_SCENE_HPP
#define VOLCANO_SIMULATION_SCENE_HPP

#include <entt/entt.hpp>

#include <Volcano/Simulation/Common.hpp>
#include <Volcano/Simulation/Context.hpp>

VOLCANO_SIMULATION_BEGIN

class Scene: public entt::registry {
public:
    Scene();
    virtual ~Scene();

public:
    entt::entity global() const {
        return global_;
    }

private:
    entt::entity global_;
};

VOLCANO_SIMULATION_END

#endif // VOLCANO_SIMULATION_SCENE_HPP
