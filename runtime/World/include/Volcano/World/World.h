//
//
#ifndef VOLCANO_WORLD_WORLD_H
#define VOLCANO_WORLD_WORLD_H

#include <memory>
#include <string>

#include <Volcano/Version.h>
#include <Volcano/World/Common.h>
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

class World {
public:
	World();
	virtual ~World() = default;

public:
	Scene& scene() noexcept {
		return *scene_;
	}

	const Scene& scene() const noexcept {
		return *scene_;
	}

	virtual const std::string& name() const noexcept = 0;
	virtual const VersionNumber& version() const noexcept = 0;
	virtual void update(Clock::duration elapsed) noexcept;

private:
	std::unique_ptr<Scene> scene_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_WORLD_H
