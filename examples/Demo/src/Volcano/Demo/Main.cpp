//
//
#include <SDL_main.h>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Game/World.h>
#include <Volcano/System/Frontend.h>

#include "Volcano/Demo/Common.h"

VOLCANO_DEMO_BEGIN

static bool pollEvents(System::Frontend& frontend) {
    SDL_Event evt;

    while (SDL_PollEvent(&evt)) {
        if (evt.type == SDL_QUIT) {
            return false;
        }
        frontend.feedEvent(evt);
    }

    return true;
}

static int run(int argc, char* argv[]) {
    int ret = SDL_Init(SDL_INIT_EVERYTHING);
    if (ret < 0) {
        return EXIT_FAILURE;
    }

    auto sdl_guard = scopeGuard([] {
        SDL_Quit();
    });

#ifdef VOLCANO_DEBUG
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

    Game::World game_world;
    System::Frontend frontend(game_world);

    frontend.init("VolcanoDemo", 800, 600);

    while (!frontend.shouldQuit()) {
        pollEvents(frontend);
        frontend.update(10ms);
        SDL_Delay(10);
    }

    return EXIT_SUCCESS;
}

VOLCANO_DEMO_END

int main(int argc, char* argv[]) {
    return Volcano::Demo::run(argc, argv);
}
