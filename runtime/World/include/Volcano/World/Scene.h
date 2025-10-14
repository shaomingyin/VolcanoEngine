//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <entt/entt.hpp>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Scene: public entt::registry {

public:
    Scene();
    virtual ~Scene() = default;

public:
    entt::entity global() const {
        return global_;
    }

private:
    entt::entity global_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCENE_H
