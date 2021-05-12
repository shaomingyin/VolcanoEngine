//
//
#ifndef VOLCANO_LAUNCHER_PLAYER_HPP
#define VOLCANO_LAUNCHER_PLAYER_HPP

#include <string_view>

#include <Volcano/FileSystem.hpp>
#include <Volcano/Launcher/Common.hpp>
#include <Volcano/Launcher/Window.hpp>

VOLCANO_LAUNCHER_BEGIN

class Player {
	VOLCANO_DISABLE_COPY_AND_MOVE(Player)

public:
	Player(FileSystem *fs);
	virtual ~Player(void);

public:
	virtual bool init(void);
	bool shouldQuit(void) const;
	virtual void handleEvent(const SDL_Event &evt);
	TimePoint update(void);

protected:
	FileSystem *fs(void);
	Window &window(void);
	void quit(void);
	virtual void frame(Duration elapsed);

private:
	FileSystem *m_fs;
	Window m_window;
	bool m_shoudlQuit;
	TimePoint m_last;
	Duration m_elapsedMin;
	int m_fps;
	int m_fpsCount;
	TimePoint m_fpsLast;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_PLAYER_HPP
