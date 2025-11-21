//
//
#ifndef VOLCANO_WORLD_CONTEXT_HPP
#define VOLCANO_WORLD_CONTEXT_HPP

#include <chrono>

#include <async++.h>

#include <Volcano/World/Common.hpp>

VOLCANO_WORLD_BEGIN

class Context {
public:
    using Clock = std::chrono::steady_clock;

public:
    Context() = default;
    virtual ~Context() = default;

public:
    virtual void schedule(async::task_run_handle t) = 0;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CONTEXT_HPP
