//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <rttr/type>
#include <entt/entt.hpp>

#include <Volcano/World/Common.h>
#include <Volcano/World/Physics.h>

VOLCANO_WORLD_BEGIN

class Scene: public entt::registry {
    RTTR_ENABLE()

public:
    Scene();
	virtual ~Scene() = default;

public:
    Physics& physics() noexcept {
        return physics_;
    }

    const Physics& physics() const noexcept {
        return physics_;
    }

    virtual entt::entity mainCamera() const noexcept = 0;
	virtual void update(Clock::duration elapsed) noexcept;

private:
    void onEntityAdded(entt::entity ent) noexcept;
    void onEntityRemoved(entt::entity ent) noexcept;

private:
    Physics physics_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCENE_H
