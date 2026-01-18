//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <string>
#include <memory>
#include <vector>

#include <Volcano/Graphics/View.hpp>
#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Dynamic.hpp>
#include <Volcano/Game/Camera.hpp>
#include <Volcano/Game/Light.hpp>
#include <Volcano/Game/DirectionalLight.hpp>
#include <Volcano/Game/Scene.hpp>
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

class World: public Object {
public:
    using Scenes = std::vector<std::unique_ptr<Scene>>;

public:
    World();
    virtual ~World();

public:
    Dynamic& dynamic() noexcept {
        return dynamic_;
    }

    const Dynamic& dynamic() const noexcept {
        return dynamic_;
    }

    Camera& mainCamera() noexcept {
        return main_camera_;
    }

    const Camera& mainCamera() const noexcept {
        return main_camera_;
    }

    Light& ambientLight() noexcept {
        return ambient_light_;
    }

    const Light& ambientLight() const noexcept {
        return ambient_light_;
    }

    DirectionalLight& sunLight() noexcept {
        return sun_light_;
    }

    const DirectionalLight& sunLight() const noexcept {
        return sun_light_;
    }

    Scenes& scenes() noexcept {
        return scenes_;
    }

    const Scenes& scenes() const noexcept {
        return scenes_;
    }

    Graphics::View buildView(Scheduler& scheduler) const;

protected:
    void update(Clock::duration elapsed, Scheduler& scheduler) override;
    void buildVisibleSet(Graphics::VisibleSet& visible_set, Scheduler& scheduler) const override;

private:
    Dynamic dynamic_;
    Camera main_camera_;
    Light ambient_light_;
    DirectionalLight sun_light_;
    Scenes scenes_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
