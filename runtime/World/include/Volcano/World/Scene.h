//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <memory>
#include <string>
#include <filesystem>

#include <rttr/type>
#include <entt/entt.hpp>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Scene: public entt::registry {
    RTTR_ENABLE()

public:
    Scene();
	virtual ~Scene() = default;

public:
    virtual entt::entity mainCamera() const noexcept;
	virtual void update(Clock::duration elapsed) noexcept;

protected:
    virtual void onEntityAdded(entt::entity ent) noexcept;
    virtual void onEntityRemoved(entt::entity ent) noexcept;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCENE_H
