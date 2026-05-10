//
//
#ifndef VOLCANO_STAGE_SCENE_H
#define VOLCANO_STAGE_SCENE_H

#include <chrono>

#include <entt/entt.hpp>

#include <Volcano/Stage/Common.h>

VOLCANO_STAGE_BEGIN

class Scene: public entt::registry {
public:
    using Clock = std::chrono::steady_clock;

public:
    Scene();
    virtual ~Scene();

private:
    void onEntityAdded(entt::registry& registry, entt::entity entity);
};

VOLCANO_STAGE_END

#endif // VOLCANO_STAGE_SCENE_H
