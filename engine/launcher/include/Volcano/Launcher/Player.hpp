//
//
#ifndef VOLCANO_LAUNCHER_PLAYER_HPP
#define VOLCANO_LAUNCHER_PLAYER_HPP

#include <string_view>

#include <GL/gl3w.h>

#include <Volcano/FileSystem.hpp>
#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

class Player {
	VOLCANO_DISABLE_COPY_AND_MOVE(Player)

public:
	Player(FileSystem *fs);
	virtual ~Player(void);

public:
	virtual bool init(void);
	bool shouldQuit(void) const;
	TimePoint update(void);

protected:
	FileSystem *fs(void);
	void quit(void);
	virtual void frame(Duration elapsed);
	
private:
	static void onSize(GLFWwindow *window, int width, int height);
	static void onKey(GLFWwindow *window, int keyCode, int scanCode, int action, int mods);
	static void onMouseButton(GLFWwindow *window, int button, int action, int mods);
	static void onMouseMove(GLFWwindow *, double, double);

private:
	FileSystem *m_fs;
	GLFWwindow *m_window;
	int m_size[2];
	bool m_visible;
	union GL3WProcs m_gl3w;
	Graphics::Renderer m_renderer;
	bool m_shoudlQuit;
	TimePoint m_last;
	Duration m_elapsedMin;
	int m_fps;
	int m_fpsCount;
	TimePoint m_fpsLast;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_PLAYER_HPP
