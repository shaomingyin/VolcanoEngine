//
//
#ifndef VOLCANO_SCENE_REGISTRY_HPP
#define VOLCANO_SCENE_REGISTRY_HPP

#include <entt/entt.hpp>

#include <Volcano/Scene/Common.hpp>

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

#endif // VOLCANO_SCENE_REGISTRY_HPP
