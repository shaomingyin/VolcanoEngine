//
//
#ifndef VOLCANO_SYSTEM_LOCAL_H
#define VOLCANO_SYSTEM_LOCAL_H

#include <Volcano/Acoustics/Space.h>
#include <Volcano/Graphics/Renderer.h>

#include <Volcano/System/Common.h>
#include <Volcano/System/Window.h>
#include <Volcano/System/Base.h>

VOLCANO_SYSTEM_BEGIN

class Local: public Base {
public:
	Local();
	~Local() override = default;

protected:
	void loadConfig(const nlohmann::json& json) override;
	void loadMap(const nlohmann::json& json, entt::handle entity) override;
	void loadEntity(const nlohmann::json& json, entt::handle entity) override;
	void tick(Duration elapsed) override;
	void frame(Duration elapsed) override;
	virtual void loadingFrame(Duration elapsed);
	virtual void readyFrame(Duration elapsed);
	virtual void playingFrame(Duration elapsed);
	virtual void pausedFrame(Duration elapsed);
	virtual void stoppingFrame(Duration elapsed);
	virtual void errorFrame(Duration elapsed);
	virtual void handleEvent(const SDL_Event& evt);

private:
	void pollEvents();

private:
	void runGui();
	void runMainMenuBar();
	void runFps();
	void runConsole();
	void runOptions();
	void runAbout();

private:
	Window window_;
	bool gui_show_;
	bool gui_show_fps_;
	bool gui_show_console_;
	bool gui_show_demo_;
	Acoustics::Space acoustics_space_;
	Graphics::Renderer graphics_renderer_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_LOCAL_H
