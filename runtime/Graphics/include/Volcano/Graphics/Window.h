//
//
#ifndef VOLCANO_GRAPHICS_WINDOW_H
#define VOLCANO_GRAPHICS_WINDOW_H

#include <string>

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Target.h>

VOLCANO_GRAPHICS_BEGIN

class Window: public Target {
protected:
    Window(SDL_Window* handle);

public:
    virtual ~Window();

public:
    SDL_Window* handle() {
        return handle_;
    }

    void show() {
        SDL_ShowWindow(handle_);
    }

    void hide() {
        SDL_HideWindow(handle_);
    }

    bool isVisible() const {
        return isEnabled() && visible_;
    }

    void resize(int width, int height) override;
    virtual void handleEvent(const SDL_Event& evt);

private:
    SDL_Window* handle_;
    Uint32 id_;
    bool visible_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_WINDOW_H
