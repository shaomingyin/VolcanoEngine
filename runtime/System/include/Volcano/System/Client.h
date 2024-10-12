//
//
#ifndef VOLCANO_SYSTEM_CLIENT_H
#define VOLCANO_SYSTEM_CLIENT_H

#include <memory>
#include <map>
#include <string>

#include <Volcano/Game/Context.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Client: public Game::Context {
public:
    Client();
    virtual ~Client();

public:
    virtual bool init(const std::string& title, int width, int height);
    bool shouldQuit();
    void update(Duration elapsed);

    uint64_t fpsMax() const {
        return 60; // TODO
    }

    uint64_t fps() const {
        return frame_count_per_second_;
    }

protected:
    virtual void frame(Duration elapsed) = 0;

private:
    TimePoint last_frame_count_time_point_;
    uint64_t frame_count_;
    uint64_t frame_count_per_second_;
    GLFWwindow* window_;
    std::unique_ptr<Graphics::Renderer> graphics_renderer_;
    ENetHost enet_host_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_CLIENT_H
