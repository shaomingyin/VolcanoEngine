//
//
#include <string>
#include <memory>
#include <thread>

#include <argh.h>
#include <SDL.h>
#include <physfs.h>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Launcher/Common.hpp>
#include <Volcano/Launcher/Application.hpp>

VOLCANO_LAUNCHER_BEGIN

static int main(int argc, char *argv[])
{
    int ret;

    ret = PHYSFS_init(argv[0]);
    if (ret < 0)
        return EXIT_FAILURE;

    ScopeGuard physfsGuard([] { PHYSFS_deinit(); });

    ret = SDL_Init(SDL_INIT_EVERYTHING);
    if (ret != 0)
        return EXIT_FAILURE;

    ScopeGuard sdlGuard([] { SDL_Quit(); });

    auto app = std::make_unique<Application>();
    if (!app || !app->init())
        return EXIT_FAILURE;

    SDL_Event event;
    Uint32 curr, pass;
    Uint32 last = SDL_GetTicks();

    for (;;) {
        while (SDL_PollEvent(&event)) {
            if (event.type != SDL_QUIT)
                app->handleEvent(event);
            else
                return !app->canQuit();
        }
        curr = SDL_GetTicks();
        if (curr > last) {
            pass = curr - last;
            if (pass >= 16) {
                app->frame(float(pass) / 1000.0f);
                last = curr;
            } else
                SDL_Delay(16 - pass);
        } else if (curr < last)
            last = curr;
    }

    return EXIT_SUCCESS;
}

VOLCANO_LAUNCHER_END

int main(int argc, char *argv[])
{
    return Volcano::Launcher::main(argc, argv);
}
