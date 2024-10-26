//
//
#ifndef VOLCANO_SYSTEM_FRONTEND_H
#define VOLCANO_SYSTEM_FRONTEND_H

#include <atomic>
#include <memory>
#include <string>

#include <taskflow/taskflow.hpp>

#include <Volcano/ResourceManager.h>
#include <Volcano/Game/World.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Frontend {
public:
    Frontend(ResourceManager& resource_manager);
    virtual ~Frontend();

public:
    bool init(const std::string& title, int width, int height);
    void bind(Game::World* game_world);
    void handleEvent(const SDL_Event& evt);
    void frame(Duration elapsed);

    bool shouldQuit() const {
        return bool(flags_ & FlagQuit);
    }

private:
    void buildGraphicsView();

private:
    enum {
        FlagQuit = 0x1,
        FlagWindowVisible = 0x2
    };

    ResourceManager& resource_manager_;
    Game::World* game_world_;

    int flags_;
    SDL_Window* window_;
    Uint32 window_id_;
    SDL_GLContext gl_context_;
    Graphics::Renderer graphics_renderer_;
    Graphics::View views_[2];
    std::atomic_int current_view_;
    tf::Executor executor_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_FRONTEND_H
