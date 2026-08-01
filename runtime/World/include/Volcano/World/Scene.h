//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <string>

#include <entt/entt.hpp>

#include <Volcano/Version.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Scene: public entt::registry {
public:
    Scene();
    virtual ~Scene();

public:
    const std::string& name() const noexcept {
        return name_;
    }

    void setName(const std::string& v) noexcept {
        name_ = v;
    }

    const std::string& description() const noexcept {
        return description_;
    }

    void setDescription(const std::string& v) noexcept {
        description_ = v;
    }

    const VersionNumber& version() const noexcept {
        return version_;
    }

    void setVersion(const VersionNumber& v) noexcept {
        version_ = v;
    }

    virtual void update(Clock::duration elapsed) noexcept = 0;
    virtual entt::entity player() const noexcept = 0;

private:
    void handleEntityAdded(entt::entity ent) noexcept;
    void handleEntityRemoved(entt::entity ent) noexcept;

private:
    std::string name_;
    std::string description_;
    VersionNumber version_;
};

VOLCANO_WORLD_END

extern std::unique_ptr<Volcano::World::Scene> volcanoCreateGame();

#endif // VOLCANO_WORLD_SCENE_H
