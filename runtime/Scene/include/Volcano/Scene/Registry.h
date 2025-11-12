//
//
#ifndef VOLCANO_SCENE_REGISTRY_H
#define VOLCANO_SCENE_REGISTRY_H

#include <entt/entt.hpp>

#include <Volcano/Scene/Common.h>

VOLCANO_SCENE_BEGIN

class Registry: public entt::registry {

public:
    Registry();
    virtual ~Registry() = default;

public:
    entt::entity global() const {
        return global_;
    }

private:
    entt::entity global_;
};

VOLCANO_SCENE_END

#endif // VOLCANO_SCENE_REGISTRY_H
