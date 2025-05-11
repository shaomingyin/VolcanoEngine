/*
 *
 */
#ifndef VOLCANO_FRAMEWORK_GAME_H
#define VOLCANO_FRAMEWORK_GAME_H

#include <string>

#include <rttr/type>
#include <nlohmann_json.h>
#include <SDL3/SDL_events.h>

#include <Volcano/VersionNumber.h>
#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/Context.h>

VOLCANO_FRAMEWORK_BEGIN

class Game {
    RTTR_ENABLE()

public:
    Game(Context& context);

public:
    Context& context() {
        return context_;
    }

    const Context& context() const {
        return context_;
    }

    virtual const std::string& name();
    virtual const VersionNumber& version() const;
    virtual void loadConfig(const nlohmann::json& j);
    virtual void loadScene(const nlohmann::json& j);
    virtual void event(const SDL_Event& evt);
    virtual void frame(Duration elapsed);

private:
    Context& context_;
};

VOLCANO_FRAMEWORK_END

#endif /* VOLCANO_FRAMEWORK_GAME_H */
