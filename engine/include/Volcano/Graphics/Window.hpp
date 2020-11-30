//
//
#ifndef VOLCANO_GRAPHICS_WINDOW_HPP
#define VOLCANO_GRAPHICS_WINDOW_HPP

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Surface.hpp>

VOLCANO_GRAPHICS_BEGIN

class Window: public Surface {
public:
    Window(void);
    ~Window(void) override;

public:
    bool init(const char *name, int width, int height, int flags = 0);
    void shutdown(void);
    bool handleEvent(const SDL_WindowEvent &evt);
	Uint32 id(void) const;
	bool visible(void) const;
	void show(void);
	void hide(void);
    bool isValid(void) override;
	void resize(int width, int height) override;
    bool activate(void) override;
    void deactivate(void) override;

private:
	SDL_Window *m_handle;
	Uint32 m_id;
	bool m_visible;
    SDL_GLContext m_glContext;
	union GL3WProcs m_gl3w;
};

VOLCANO_INLINE Uint32 Window::id(void) const
{
	return m_id;
}

VOLCANO_INLINE bool Window::visible(void) const
{
	return m_visible;
}

VOLCANO_INLINE void Window::show(void)
{
	VOLCANO_ASSERT(m_handle != nullptr);
	SDL_ShowWindow(m_handle);
}

VOLCANO_INLINE void Window::hide(void)
{
	VOLCANO_ASSERT(m_handle != nullptr);
	SDL_HideWindow(m_handle);
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_WINDOW_HPP
