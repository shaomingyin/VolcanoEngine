//
//
#ifndef VOLCANO_SYSTEM_LOCAL_H
#define VOLCANO_SYSTEM_LOCAL_H

#include <memory>

#include <Volcano/Error.h>
#include <Volcano/Graphics/Renderer.h>
//#include <Volcano/Graphics/BulletDebugDrawer.h>
#include <Volcano/Acoustics/Space.h>
#include <Volcano/Gui/Window.h>
#include <Volcano/System/Common.h>
#include <Volcano/System/Window.h>
#include <Volcano/System/Input.h>
#include <Volcano/System/Console.h>
#include <Volcano/System/Base.h>

VOLCANO_SYSTEM_BEGIN

class Local: public Base {
public:
	Local();
	virtual ~Local();

protected:
    void handleEvent(const SDL_Event& evt);
    void loadingFrame(Duration elapsed) override;
    void readyFrame(Duration elapsed) override;
	void playingFrame(Duration elapsed) override;
    void pausedFrame(Duration elapsed) override;
    void stoppingFrame(Duration elapsed) override;
    void errorFrame(Duration elapsed) override;
    void loadEntity(entt::handle ent, const nlohmann::json& json) override;

private:
    Window window_;
    Input input_;
    Graphics::Renderer graphics_renderer_;
    //Graphics::BulletDebugDrawer bullet_debug_drawer_;
    Acoustics::Space acoustics_space_;
    Console console_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_LOCAL_H
