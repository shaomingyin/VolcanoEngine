//
//
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_messagebox.h>

#include <argh.h>
#include <rttr/type>

#include <Volcano/Launcher/Common.h>
#include <Volcano/Launcher/Application.h>

VOLCANO_LAUNCHER_BEGIN

static void init(Application** appstate, const argh::parser& args) {
    if (args.size() > 0) {
        if (args.size() > 1) {
            throw std::runtime_error("Too many app.");
        }
    }

    std::string game_path;
    args(0, std::format("{}/Game", SDL_GetBasePath())) >> game_path;
    rttr::library game(game_path);
    game.load();
    rttr::type game_type = rttr::type::get<void>();
    for (auto type : game.get_types()) {
        if (type.is_derived_from<Volcano::Framework::Game>()) {
            game_type = type;
            break;
        }
    }
    if (game_type == rttr::type::get<void>()) {
        throw std::runtime_error("No game found.");
    }

    std::string plugins_path;
    args({ "-p", "--plugins" }, std::format("{}/Plguins/", SDL_GetBasePath())) >> plugins_path;
    for (auto dent : std::filesystem::directory_iterator(plugins_path)) {
        if (dent.is_regular_file()) {
            rttr::library(dent.path().generic_string()).load();
        }
    }

    *appstate = new Application( game_type,
        SDL_OpenTitleStorage(nullptr, 0),
        SDL_OpenUserStorage("VolcanoEngine", "VolcanoLauncher", 0));
}

VOLCANO_LAUNCHER_END

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
        Volcano::Launcher::init(
            reinterpret_cast<Volcano::Launcher::Application**>(appstate),
            argh::parser(argv));
    } catch (std::exception& e) {
        SDL_Quit();
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "VolcanoError", e.what(), NULL);
        return SDL_APP_FAILURE;
    }

    //SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, "144");

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    return reinterpret_cast<Volcano::Launcher::Application*>(appstate)->event(*event);
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    return reinterpret_cast<Volcano::Launcher::Application*>(appstate)->update();
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    if (result == SDL_APP_CONTINUE) {
        delete reinterpret_cast<Volcano::Launcher::Application*>(appstate);
    }
}
