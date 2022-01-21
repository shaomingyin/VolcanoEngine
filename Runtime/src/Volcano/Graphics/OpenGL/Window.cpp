//
//
#include <Volcano/Graphics/OpenGL/Window.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

Window::Window(void)
{
}

Window::~Window(void)
{
}

void Window::initializeGL(void)
{
    m_renderer.init(width(), height());
}

void Window::paintGL(void)
{
    m_renderer.beginFrame();
    m_renderer.endFrame();
}

void Window::resizeGL(int w, int h)
{
    m_renderer.resize(w, h);
}

VOLCANO_GRAPHICS_OPENGL_END
