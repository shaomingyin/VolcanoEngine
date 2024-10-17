//
//
#include <SDL_main.h>

#include <Volcano/Game/Context.h>
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
    Game::Context game_context;
    System::Frontend frontend(game_context);

    //game_context.init();
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
