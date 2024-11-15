//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Scene: public entt::registry {
public:
	Scene();
	Scene(const Scene&) = delete;
	Scene(Scene&&) = delete;
	virtual ~Scene();

public:
	Scene& operator=(const Scene&) = delete;
	Scene& operator=(Scene&&) = delete;

	entt::handle global() const {
		return global_;
	}

	entt::handle createEntity() {
		return entt::handle(*this, create());
	}

private:
	entt::handle global_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCENE_H
