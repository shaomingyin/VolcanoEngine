//
//
#ifndef VOLCANO_OPENGL_WINDOW_H
#define VOLCANO_OPENGL_WINDOW_H

#include <Volcano/Graphics/Window.h>
#include <Volcano/OpenGL/Common.h>

VOLCANO_OPENGL_BEGIN

class Window: public Graphics::Window {
public:
    Window(const std::string& title, int width, int height);
    ~Window() override;

public:

private:
    SDL_GLContext gl_;
    GL3WProcs gl3w_;
};

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_WINDOW_H
