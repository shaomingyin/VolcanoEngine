//
//
#ifndef VOLCANO_CLIENT_WINDOW_HPP
#define VOLCANO_CLIENT_WINDOW_HPP

#include <string>

#include <GL/gl3w.h>

#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Client/Common.hpp>

VOLCANO_CLIENT_BEGIN

class Window: public Napi::ObjectWrap<Window> {
	VOLCANO_DISABLE_COPY_AND_MOVE(Window)

public:
	Window(const Napi::CallbackInfo &info);
	virtual ~Window(void);

public:
	static void initExports(Napi::Env env, Napi::Object exports);

private:
	Napi::Value init(const Napi::CallbackInfo &info);
	Napi::Value connect(const Napi::CallbackInfo &info);
	Napi::Value isVisible(const Napi::CallbackInfo &info);
	void setVisible(const Napi::CallbackInfo &info, const Napi::Value &value);
	Napi::Value isFullScreen(const Napi::CallbackInfo &info);
	void setFullScreen(const Napi::CallbackInfo &info, const Napi::Value &value);
	Napi::Value title(const Napi::CallbackInfo &info);
	void setTitle(const Napi::CallbackInfo &info, const Napi::Value &value);
	Napi::Value position(const Napi::CallbackInfo &info);
	void setPosition(const Napi::CallbackInfo &info, const Napi::Value &value);
	Napi::Value size(const Napi::CallbackInfo &info);
	void setSize(const Napi::CallbackInfo &info, const Napi::Value &value);

private:
	void update(void);
	static void onPos(GLFWwindow *window, int x, int y);
	static void onSize(GLFWwindow *window, int width, int height);
	static void onKey(GLFWwindow *window, int keyCode, int scanCode, int action, int mods);
	static void onMouseButton(GLFWwindow *window, int button, int action, int mods);
	static void onMouseMove(GLFWwindow *window, double x, double y);
	static void onScroll(GLFWwindow *window, double x, double y);
	static void connectionAllocCallback(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
	static void connectionReceiveCallback(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags);

private:
	GLFWwindow *m_window;
	std::string m_title;
	int m_rect[4];
	bool m_visible;
	union GL3WProcs m_gl3w;
	Graphics::Renderer m_renderer;
	uv_udp_t m_connection;
};

VOLCANO_CLIENT_END

#endif // VOLCANO_CLIENT_WINDOW_HPP
