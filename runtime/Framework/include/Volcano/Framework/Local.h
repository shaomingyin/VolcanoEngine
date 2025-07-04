/*
 *
 */
#ifndef VOLCANO_FRAMEWORK_LOCAL_H
#define VOLCANO_FRAMEWORK_LOCAL_H

#include <SDL3/SDL_events.h>

#include <Volcano/Acoustics/Space.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/Window.h>
#include <Volcano/Framework/Base.h>

VOLCANO_FRAMEWORK_BEGIN

class Local: public Base {
public:
    Local(SDL_Storage* rootfs, Game& game);

public:
    virtual void event(const SDL_Event& evt);

protected:
    void frame(Duration elapsed) override;
    void loadConfig(const nlohmann::json& j) override;
    void loadScene(const nlohmann::json& j) override;

private:
    void initAcoustics();
    void initGraphics();
    void initGui();

private:
    Window window_;
    Acoustics::Space* acoustics_space_;
    Graphics::Renderer* graphics_renderer_;
};

VOLCANO_FRAMEWORK_END

#endif /* VOLCANO_FRAMEWORK_LOCAL_H */
