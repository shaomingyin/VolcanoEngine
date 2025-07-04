//
//
#ifndef VOLCANO_FRAMEWORK_CONTEXT_H
#define VOLCANO_FRAMEWORK_CONTEXT_H

#include <memory>

#include <entt/entt.hpp>
#include <nlohmann_json.h>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_storage.h>

#include <Volcano/World/Scene.h>
#include <Volcano/Framework/Common.h>

VOLCANO_FRAMEWORK_BEGIN

class Context {
public:
    Context(SDL_Storage* rootfs, SDL_Storage* userfs);
    virtual ~Context() = default;

public:
    SDL_Storage* rootfs() noexcept {
        return rootfs_;
    }

    SDL_Storage* userfs() noexcept {
        return userfs_;
    }

    void schedule(async::task_run_handle t) {
        scheduler_.schedule(std::move(t));
    }

    virtual void update(Duration elapsed);

private:
    async::fifo_scheduler scheduler_;
    SDL_Storage* rootfs_;
    SDL_Storage* userfs_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_CONTEXT_H
