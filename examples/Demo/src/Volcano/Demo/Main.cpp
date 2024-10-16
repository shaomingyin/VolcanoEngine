//
//
#include <SDL_main.h>

#include <Volcano/System/Server.h>
#include <Volcano/System/Client.h>

#include "Volcano/Demo/Common.h"

VOLCANO_DEMO_BEGIN

static bool pollEvents(System::Client& client) {
    SDL_Event evt;

    while (SDL_PollEvent(&evt)) {
        if (evt.type == SDL_QUIT) {
            return false;
        }
        client.feedEvent(evt);
    }
}

static int run(int argc, char* argv[]) {
    System::Server server;
    System::Client client(server);

    server.init();
    client.init("VolcanoDemo", 800, 600);

    while (!client.shouldQuit()) {
        pollEvents(client);
        client.update(10ms);
        SDL_Delay(10);
    }

    return EXIT_SUCCESS;
}

VOLCANO_DEMO_END

int main(int argc, char* argv[]) {
    return Volcano::Demo::run(argc, argv);
}
