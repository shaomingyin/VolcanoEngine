/*
 *
 */
#ifndef VOLCANO_LAUNCHER_APPLICATION_H
#define VOLCANO_LAUNCHER_APPLICATION_H

#include <memory>

#include <rttr/type.h>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_events.h>

#include <Volcano/Schema/Common.h>
#include <Volcano/Physics/World.h>
#include <Volcano/Acoustics/Space.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Framework/Context.h>
#include <Volcano/Framework/Game.h>
#include <Volcano/Launcher/Common.h>

VOLCANO_LAUNCHER_BEGIN

class Application: public Framework::Context {
public:
    Application(rttr::type game_type, SDL_Storage* rootfs, SDL_Storage* userfs);

public:
    SDL_AppResult event(const SDL_Event& evt);
    SDL_AppResult update();

protected:
    void loadConfig(const nlohmann::json& scene);
    void loadScene(const nlohmann::json& scene);

private:
    rttr::type game_type_;
    async::task<void> loading_task_;
    std::unique_ptr<Framework::Game> game_;
    std::unique_ptr<Physics::World> physics_world_;
    std::unique_ptr<Acoustics::Space> acoustics_space_;
    std::unique_ptr<Graphics::Renderer> graphics_renderer_;
    std::unique_ptr<Graphics::Window> graphics_window_;
    TimePoint frame_last_;
};

VOLCANO_LAUNCHER_END

#endif /* VOLCANO_LAUNCHER_APPLICATION_H */
