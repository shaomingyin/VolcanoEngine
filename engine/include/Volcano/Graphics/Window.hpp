//
//
#ifndef VOLCANO_GRAPHICS_WINDOW_HPP
#define VOLCANO_GRAPHICS_WINDOW_HPP

#include <string>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Renderer.hpp>

VOLCANO_GRAPHICS_BEGIN

class Window: public Napi::ObjectWrap<Window> {
	VOLCANO_DISABLE_COPY_AND_MOVE(Window)

public:
	Window(const Napi::CallbackInfo &info);
	virtual ~Window(void);

public:
	static Napi::Function defineClass(Napi::Env env);

public:
	sigslot::signal<bool> onVisibleChanged;
	sigslot::signal<bool> onFullScreenChanged;
	sigslot::signal<std::string_view> onTitleChanged;
	sigslot::signal<int, int> onPositionChanged;
	sigslot::signal<int, int> onSizeChanged;

private:
	Napi::Value renderer(const Napi::CallbackInfo &info);
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
	Napi::Value isGrabMouse(const Napi::CallbackInfo &info);
	void setGrabMouse(const Napi::CallbackInfo &info, const Napi::Value &value);
	Napi::Value fps(const Napi::CallbackInfo &info);
	Napi::Value fpsMax(const Napi::CallbackInfo &info);
	void setFpsMax(const Napi::CallbackInfo &info, const Napi::Value &value);

private:
	void setFpsMax(int v);
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
	GLFWwindow *m_handle;
	std::string m_title;
	int m_rect[4];
	bool m_isVisible;
	bool m_isGrabMouse;
	union GL3WProcs m_gl3w;
	Napi::ObjectReference m_rendererRef;
	Renderer *m_renderer;
	Duration m_elapsedMin;
	TimePoint m_frameLast;
	TimePoint m_frameCountLast;
	int m_frameCount;
	int m_frameCountPerSecond;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_WINDOW_HPP
