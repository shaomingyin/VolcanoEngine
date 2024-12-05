//
//
#ifndef VOLCANO_SYSTEM_LOCAL_H
#define VOLCANO_SYSTEM_LOCAL_H

#include <SDL.h>

#include <Volcano/Acoustics/Space.h>
#include <Volcano/Graphics/Renderer.h>

#include <Volcano/System/Common.h>
#include <Volcano/System/Window.h>
#include <Volcano/System/Console.h>
#include <Volcano/System/Base.h>

VOLCANO_SYSTEM_BEGIN

class Local: public Base {
public:
	Local(const std::string& manifest_path);
	~Local() override = default;

protected:
	void loadMap(const nlohmann::json& json, entt::handle entity) override;
	void loadEntity(const nlohmann::json& json, entt::handle entity) override;
	void loadingFrame(Duration elapsed) override;
	void readyFrame(Duration elapsed) override;
	void playingFrame(Duration elapsed) override;
	void pausedFrame(Duration elapsed) override;
	void errorFrame(Duration elapsed) override;
	virtual void handleEvent(const SDL_Event& evt);

private:
	void pollEvents();
	void render();

private:
	Window window_;
	Acoustics::Space acoustics_space_;
	Graphics::Renderer graphics_renderer_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_LOCAL_H
