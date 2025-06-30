/*
 *
 */
#ifndef VOLCANO_FRAMEWORK_LOCAL_H
#define VOLCANO_FRAMEWORK_LOCAL_H

#include <SDL3/SDL_events.h>

#include <Volcano/Acoustics/Space.h>
#include <Volcano/Graphics/Device.h>
#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/Base.h>

VOLCANO_FRAMEWORK_BEGIN

class Local: public Base {
public:
    Local(SDL_Storage* rootfs, SDL_Storage* userfs);

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
    rttr::variant acoustics_space_instance_;
    Acoustics::Space* acoustics_space_;
    rttr::variant graphics_device_instance_;
    Graphics::Device* graphics_device_;
    Graphics::Window* graphics_window_;
};

VOLCANO_FRAMEWORK_END

#endif /* VOLCANO_FRAMEWORK_LOCAL_H */
