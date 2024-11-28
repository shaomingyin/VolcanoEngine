//
//
#ifndef VOLCANO_PLAYER_WINDOW_H
#define VOLCANO_PLAYER_WINDOW_H

#include <GL/gl3w.h>

#include <Volcano/Graphics/Target.h>
#include <Volcano/Player/Common.h>

VOLCANO_PLAYER_BEGIN

class Window: public Graphics::Target {
public:
    Window(const std::string& title, int x, int y, int w, int h);
    virtual ~Window();

public:
    SDL_Window* handle() {
        return handle_;
    }

    int x() const {
        return x_;
    }

    int y() const {
        return y_;
    }

    void move(int x, int y) {
        SDL_SetWindowPosition(handle_, x, y);
    }

    bool isVisible() const {
        return bool(state_ & StateVisible);
    }

    void show() {
        SDL_ShowWindow(handle_);
    }

    void hide() {
        SDL_HideWindow(handle_);
    }

    bool beginFrame() override;
    void endFrame() override;
    virtual void handleEvent(const SDL_Event& evt);
    void toggleFullScreen();

private:
    enum {
        StateVisible = 0x1,
        StateFullScreen = 0x2
    };

    int state_;
    SDL_Window* handle_;
    Uint32 id_;
    int x_;
    int y_;
    SDL_GLContext gl_context_;
    GL3WProcs gl3w_;
};

VOLCANO_PLAYER_END

#endif // VOLCANO_PLAYER_WINDOW_H
