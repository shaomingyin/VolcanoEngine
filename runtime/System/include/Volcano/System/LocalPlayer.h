//
//
#ifndef VOLCANO_SYSTEM_LOCALPLAYER_H
#define VOLCANO_SYSTEM_LOCALPLAYER_H

#include <memory>

#include <Volcano/Game/World.h>
#include <Volcano/System/Common.h>
#include <Volcano/System/FrontEnd.h>
#include <Volcano/System/Base.h>

VOLCANO_SYSTEM_BEGIN

class LocalPlayer: public Base {
public:
	LocalPlayer();
	virtual ~LocalPlayer() = default;

protected:
	Frontend& frontend() {
		return frontend_;
	}

	void mainLoop() override;
	void handleEvent(const SDL_Event& evt) override;
	void frame(Duration elapsed) override;
	virtual Game::World* gameWorld() = 0;

private:
	Game::World* game_world_;
	Frontend frontend_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_LOCALPLAYER_H
