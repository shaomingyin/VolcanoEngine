//
//
#ifndef VOLCANO_FRAMEWORK_BASE_H
#define VOLCANO_FRAMEWORK_BASE_H

#include <Volcano/Physics/World.h>
#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/Game.h>
#include <Volcano/Framework/Context.h>

VOLCANO_FRAMEWORK_BEGIN

class Base: public Context {
public:
    Base(SDL_Storage* rootfs, SDL_Storage* userfs);
    ~Base() override = default;

public:
    SDL_AppResult update() noexcept;

protected:
    virtual void frame(Duration elapsed);
    void loadConfig(const nlohmann::json& j);
    void loadScene(const nlohmann::json& j);

private:
    Game* game_;
    Physics::World* physics_world_;
    TimePoint frame_last_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_BASE_H
