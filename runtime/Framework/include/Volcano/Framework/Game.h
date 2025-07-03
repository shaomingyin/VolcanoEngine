/*
 *
 */
#ifndef VOLCANO_FRAMEWORK_GAME_H
#define VOLCANO_FRAMEWORK_GAME_H

#include <string>

#include <nlohmann_json.h>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_events.h>

#include <Volcano/VersionNumber.h>
#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/Context.h>

VOLCANO_FRAMEWORK_BEGIN

class Game {
public:
    Game(Context& context);
    virtual ~Game() = default;

public:
    Context& context() noexcept {
        return context_;
    }

private:
    Context& context_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_GAME_H
