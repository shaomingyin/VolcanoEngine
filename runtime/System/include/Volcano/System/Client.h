//
//
#ifndef VOLCANO_SYSTEM_CLIENT_H
#define VOLCANO_SYSTEM_CLIENT_H

#include <memory>
#include <string>

#include <Volcano/Graphics/Renderer.h>
#include <Volcano/System/Common.h>
#include <Volcano/System/ServerBase.h>

VOLCANO_SYSTEM_BEGIN

class Client {
public:
    Client(ServerBase& server_base);
    virtual ~Client();

public:
    virtual bool init(const std::string& title, int width, int height);
    virtual void feedEvent(const SDL_Event& evt);
    void update(Duration elapsed);

    bool shouldQuit() const {
        return bool(flags_ & FlagQuit);
    }

protected:
    virtual void frame(Duration elapsed);

private:
    enum {
        FlagQuit = 0x1,
        FlagWindowVisible = 0x2
    };

    ServerBase& server_base_;

    int flags_;
    SDL_Window* window_;
    Uint32 window_id_;
    SDL_GLContext gl_context_;
    Graphics::Renderer graphics_renderer_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_CLIENT_H
