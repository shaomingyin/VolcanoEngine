//
//
#ifndef VOLCANO_WORLD_LOADER_H
#define VOLCANO_WORLD_LOADER_H

#include <entt/entt.hpp>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Loader {
public:
	Loader(entt::registry& scene);
	virtual ~Loader() = default;

public:

private:
	entt::registry& scene_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_LOADER_H
