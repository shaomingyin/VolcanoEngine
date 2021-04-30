//
//
#ifndef VOLCANO_SYSTEM_WINDOW_HPP
#define VOLCANO_SYSTEM_WINDOW_HPP

#include <GL/gl3w.h>

#include <Volcano/VM/Window.hpp>
#include <Volcano/System/Common.hpp>
#include <Volcano/System/Renderer.hpp>

VOLCANO_SYSTEM_BEGIN

class Window : public VM::Window {
public:
	Window(void);
	~Window(void) override;

public:
	bool init(std::string_view title, int width, int height);
	void shutdown(void);
	void update(void) override;
	void handleEvent(const SDL_WindowEvent &event);
	bool isVisible(void) const override;
	void setVisible(bool v) override;
	const Eigen::Vector2i &size(void) const override;
	void setSize(const Eigen::Vector2i &size) override;
	bool isFullscreen(void) const override;
	void setFullscreen(bool v) override;
	const std::string &title(void) const override;
	void setTitle(std::string_view title) override;
	VM::Renderer *renderer(void) override;

private:
	static void glDebugLog(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
	
private:
	SDL_Window *m_window;
	std::string m_title;
	Uint32 m_id;
	bool m_visible;
	Eigen::Vector2i m_size;
	SDL_GLContext m_gl;
	union GL3WProcs m_gl3w;
	Renderer m_renderer;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_WINDOW_HPP
