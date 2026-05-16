//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <rttr/type>
#include <entt/entt.hpp>

#include <Volcano/World/Common.h>
#include <Volcano/World/Context.h>

VOLCANO_WORLD_BEGIN

class Scene: public entt::registry {
    RTTR_ENABLE()

public:
    Scene(Context& context);
	virtual ~Scene() = default;

public:
    Context& context() noexcept {
        return context_;
	}

	virtual void frame(Clock::duration elapsed) noexcept;

private:
    Context& context_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCENE_H
