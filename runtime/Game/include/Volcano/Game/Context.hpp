//
//
#ifndef VOLCANO_GAME_CONTEXT_HPP
#define VOLCANO_GAME_CONTEXT_HPP

#include <async++.h>

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Context {
public:
    Context() = default;
    virtual ~Context();

public:
    virtual void schedule(async::task_run_handle t) = 0;
    virtual unsigned long fps() const noexcept  = 0;
    virtual unsigned long fpsMax() const noexcept = 0;
    virtual void setFpsMax(unsigned long v) noexcept = 0;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CONTEXT_HPP
