/*
 *
 */
#ifndef VOLCANO_FRAMEWORK_LOCAL_H
#define VOLCANO_FRAMEWORK_LOCAL_H

#include <SDL3/SDL_events.h>

#include <Volcano/Acoustics/Space.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/ContextImpl.h>

VOLCANO_FRAMEWORK_BEGIN

template <typename T>
concept LocalTraits = requires {
    typename T::GraphicsRenderer;
    Graphics::Renderer<typename T::GraphicsRenderer>;
};

template <typename GameType, LocalTraits Traits>
class Local: public ContextImpl {
public:
    Local(SDL_Storage* rootfs, SDL_Storage* userfs, rttr::type game_type);

public:
    virtual void event(const SDL_Event& evt);
    virtual void frame(Duration elapsed);

protected:
    void loadConfig(const nlohmann::json& j) override;
    void loadScene(const nlohmann::json& j) override;

private:
    void initAcoustics();
    void initGraphics();
    void initGui();

private:
    rttr::variant acoustics_space_instance_;
    Acoustics::Space* acoustics_space_;
    rttr::variant graphics_renderer_instance_;
    Graphics::Renderer* graphics_renderer_;
    Graphics::Window* graphics_window_;
};

VOLCANO_FRAMEWORK_END

#endif /* VOLCANO_FRAMEWORK_LOCAL_H */
