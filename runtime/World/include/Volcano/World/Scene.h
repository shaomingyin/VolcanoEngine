//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <memory>
#include <string>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include <Volcano/Version.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Scene: public entt::registry {
public:
    Scene();
	virtual ~Scene();

public:
    virtual const std::string& name() const noexcept;
    virtual const std::string& description() const noexcept;
    virtual const VersionNumber& version() const noexcept;
    virtual entt::entity mainCamera() const noexcept;
	virtual void update(Clock::duration elapsed) noexcept;
    virtual void load(nlohmann::json&& json);

protected:
    virtual void onEntityAdded(entt::entity ent) noexcept;
    virtual void onEntityRemoved(entt::entity ent) noexcept;
};

VOLCANO_WORLD_END

extern std::unique_ptr<Volcano::World::Scene> createVolcanoWorldScene();

#endif // VOLCANO_WORLD_SCENE_H
