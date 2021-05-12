//
//
#ifndef VOLCANO_LAUNCHER_SINGLEPLAYER_HPP
#define VOLCANO_LAUNCHER_SINGLEPLAYER_HPP

#include <Volcano/Game/World.hpp>
#include <Volcano/Launcher/Common.hpp>
#include <Volcano/Launcher/Player.hpp>

VOLCANO_LAUNCHER_BEGIN

class SinglePlayer : public Player {
public:
	SinglePlayer(FileSystem *fs);
	virtual ~SinglePlayer(void);

public:
	bool init(void) override;
	void handleEvent(const SDL_Event &evt) override;

protected:
	void frame(Duration elapsed) override;

protected:
	Game::World *m_gameWorld;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_SINGLEPLAYER_HPP
