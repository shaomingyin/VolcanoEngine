//
//
#include <thread>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_messagebox.h>

#include <Volcano/Framework/Local.h>
#include <Volcano/Framework/Client.h>

using namespace std::chrono;
using namespace std::chrono_literals;

static void waitStorageReady(SDL_Storage* storage, steady_clock::duration timeout) {
    VOLCANO_ASSERT(storage != NULL);
    auto begin = steady_clock::now();
    while ((steady_clock::now() - begin) < timeout) {
        if (SDL_StorageReady(storage)) {
            break;
        }
        std::this_thread::sleep_for(100ms);
    }
    if (!SDL_StorageReady(storage)) {
        throw std::runtime_error("Storage not ready.");
    }
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
    SDL_InitFlags sdl_init_flags = SDL_INIT_AUDIO |
        SDL_INIT_VIDEO |
        SDL_INIT_JOYSTICK |
        SDL_INIT_HAPTIC |
        SDL_INIT_GAMEPAD |
        SDL_INIT_EVENTS |
        SDL_INIT_SENSOR |
        SDL_INIT_CAMERA;

    bool ret = SDL_Init(sdl_init_flags);
    if (!ret) {
        return SDL_APP_FAILURE;
    }

    try {
        auto rootfs = SDL_OpenTitleStorage(nullptr, 0);
        if (rootfs == nullptr) {
            throw std::runtime_error("Failed to open title storage.");
        }
        waitStorageReady(rootfs, 5s);

        *appstate = new Volcano::Framework::Local(rootfs);
    } catch (std::exception& e) {
        SDL_Quit();
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "VolcanoError", e.what(), NULL);
        return SDL_APP_FAILURE;
    }

    //SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, "144");

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    reinterpret_cast<Volcano::Framework::Local*>(appstate)->event(*event);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    return reinterpret_cast<Volcano::Framework::Local*>(appstate)->update();
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    if (result == SDL_APP_CONTINUE) {
        delete reinterpret_cast<Volcano::Framework::Local*>(appstate);
    }
}
