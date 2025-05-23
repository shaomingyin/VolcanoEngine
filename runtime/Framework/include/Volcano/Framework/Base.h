//
//
#ifndef VOLCANO_FRAMEWORK_BASE_H
#define VOLCANO_FRAMEWORK_BASE_H

#include <Volcano/Physics/World.h>
#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/Application.h>
#include <Volcano/Framework/Context.h>

VOLCANO_FRAMEWORK_BEGIN

template <typename T>
concept BaseTraits = requires {
    typename T::Application;
    Application<typename T::Game>;
    typename T::PhysicsWorld;
    Physics::World<typename T::PhysicsWorld>;
};

template <BaseTraits Traits>
class Base: public Context {
public:
    using Application = Traits::Application;
    using PhysicsWorld = Traits::PhysicsWorld;

public:
    ContextImpl(SDL_Storage* rootfs, SDL_Storage* userfs)
        : Context(rootfs, userfs) {
    }

    ~ContextImpl() override = default;

public:
    void event(const SDL_Event& evt) override;

protected:
    void loadConfig(const nlohmann::json& j) override;
    void loadScene(const nlohmann::json& j) override;
    void frame(Duration elapsed) override;

private:
    Application application_;
    PhysicsWorld physics_world_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_BASE_H
