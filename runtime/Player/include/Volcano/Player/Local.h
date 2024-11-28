//
//
#ifndef VOLCANO_PLAYER_LOCAL_H
#define VOLCANO_PLAYER_LOCAL_H

#include <Volcano/Acoustics/Space.h>
#include <Volcano/Graphics/Renderer.h>

#include <Volcano/Player/Common.h>
#include <Volcano/Player/Window.h>
#include <Volcano/Player/Console.h>
#include <Volcano/Player/Base.h>

VOLCANO_PLAYER_BEGIN

class Local: public Base {
public:
	Local(World::Scene& scene, int w = 800, int h = 600);
	virtual ~Local() = default;

protected:
	void loadScene(const nlohmann::json& json) override;
	void loadEntity(const nlohmann::json& json, entt::handle entity) override;
	void loadingFrame(Duration elapsed) override;
	void readyFrame(Duration elapsed) override;
	void playingFrame(Duration elapsed) override;
	void pausedFrame(Duration elapsed) override;
	void errorFrame(Duration elapsed) override;
	virtual void handleEvent(const SDL_Event& evt);

private:
	void update(Duration elapsed);
	void render();
	void pollEvents();

private:
	Window window_;
	Acoustics::Space acoustics_space_;
	Graphics::Renderer graphics_renderer_;
};

VOLCANO_PLAYER_END

#endif // VOLCANO_PLAYER_LOCAL_H
