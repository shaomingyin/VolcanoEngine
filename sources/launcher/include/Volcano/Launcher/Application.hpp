//
//
#ifndef VOLCANO_LAUNCHER_APPLICATION_HPP
#define VOLCANO_LAUNCHER_APPLICATION_HPP

#include <memory>

#include <SDL.h>
#include <GL/gl3w.h>

#include <Volcano/Framework/Client.hpp>
#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

class Application {
public:
    Application(void);
    virtual ~Application(void);

public:
    bool init(void);
    void handleEvent(const SDL_Event &event);
    void frame(float elapsed);
    bool canQuit(void);

private:
    void handleWindowEvent(const SDL_WindowEvent &evt);

private:
    SDL_Window *m_window;
    Uint32 m_windowId;
    bool m_windowVisible;
    SDL_GLContext m_gl;
    union GL3WProcs m_gl3w;
    std::unique_ptr<Framework::Client> m_client;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_APPLICATION_HPP
