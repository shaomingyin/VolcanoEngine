//
//
#ifndef VOLCANO_GAME_SCENE_HPP
#define VOLCANO_GAME_SCENE_HPP

#include <Jolt/Jolt.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Light.hpp>
#include <Volcano/Game/DirectionalLight.hpp>
#include <Volcano/Game/PointLight.hpp>
#include <Volcano/Game/SpotLight.hpp>
#include <Volcano/Game/Entity.hpp>

VOLCANO_GAME_BEGIN

class Scene {
public:
    Scene();

public:
    Light& ambientLight() noexcept {
        return ambient_light_;
    }

    const Light& ambientLight() const noexcept {
        return ambient_light_;
    }

    bool isDynamicEnabled() const noexcept {
        return dynamic_enabled_;
    }

    void enableDynamic() noexcept {
        dynamic_enabled_ = true;
    }

    void disableDynamic() noexcept {
        dynamic_enabled_ = false;
    }

    Eigen::Vector3f gravity() const noexcept {
        return gravity_;
    }

    void setGravity(const Eigen::Vector3f& v) noexcept {
        gravity_ = v;
    }

    void clear() noexcept;
    void update(Clock::duration elapsed) noexcept;

private:
    Light ambient_light_;
    std::vector<DirectionalLight> directional_lights_;
    std::vector<PointLight> point_lights_;
    std::vector<SpotLight> spot_lights_;
    std::vector<Entity> entities_;
    bool dynamic_enabled_;
    Eigen::Vector3f gravity_;
    JPH::PhysicsSystem dynamic_system_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SCENE_HPP
