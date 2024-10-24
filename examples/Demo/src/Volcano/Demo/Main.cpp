//
//
#include <SDL_main.h>

#include <Volcano/SDL_Initializer.h>
#include <Volcano/ScopeGuard.h>
#include <Volcano/System/SinglePlayer.h>

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
    SDL_Initializer sdl;

#ifdef VOLCANO_DEBUG
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

    System::SinglePlayer sp;

#if 0
    while (!frontend.shouldQuit()) {
        pollEvents(frontend);
        frontend.update(10ms);
        SDL_Delay(10);
    }
#endif

    return EXIT_SUCCESS;
}

VOLCANO_DEMO_END

int main(int argc, char* argv[]) {
    return Volcano::Demo::run(argc, argv);
}
