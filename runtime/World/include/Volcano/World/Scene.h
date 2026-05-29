//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <memory>
#include <string>
#include <filesystem>

#include <entt/entt.hpp>

#include <Volcano/World/Common.h>
#include <Volcano/World/Physics.h>

VOLCANO_WORLD_BEGIN

class Scene: public entt::registry {
public:
    enum class State {
        Idle = 0,
        Loading,
        Ready,
        Playing,
        Paused,
        Error,
        Max
    };

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

    void load(const std::filesystem::path& path);
    virtual entt::entity mainCamera() const noexcept = 0;
	virtual void update(Clock::duration elapsed) noexcept;

protected:
    virtual void onLoad(const std::filesystem::path& path);
    virtual void onEntityAdded(entt::entity ent) noexcept;
    virtual void onEntityRemoved(entt::entity ent) noexcept;

private:
    State state_;
    Physics physics_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCENE_H
