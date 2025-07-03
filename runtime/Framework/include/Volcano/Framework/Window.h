//
//
#ifndef VOLCANO_FRAMEWORK_WINDOW_H
#define VOLCANO_FRAMEWORK_WINDOW_H

#include <string>

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>

#include <Volcano/Framework/Common.h>

VOLCANO_FRAMEWORK_BEGIN

class Window {
public:
    Window(const std::string& title, int w, int h);
    virtual ~Window();

public:
    bool isVisible() const noexcept {
        return flags_ & FlagVisible;
    }

    void show();
    void hide();

    int width() const noexcept {
        return width_;
    }

    int height() const noexcept {
        return height_;
    }

    void resize(int width, int height);
    virtual void event(const SDL_Event& evt);

private:
    enum {
        FlagVisible = 0x1
    };

    SDL_Window* handle_;
    SDL_GLContext gl_context_;
    Uint32 id_;
    std::string title_;
    int width_;
    int height_;
    int flags_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_WINDOW_H
