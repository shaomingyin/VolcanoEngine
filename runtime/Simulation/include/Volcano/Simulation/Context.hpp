//
//
#ifndef VOLCANO_SIMULATION_CONTEXT_HPP
#define VOLCANO_SIMULATION_CONTEXT_HPP

#include <chrono>

#include <async++.h>

#include <Volcano/Simulation/Common.hpp>

VOLCANO_SIMULATION_BEGIN

class Context {
public:
    using Clock = std::chrono::steady_clock;

public:
    Context() = default;
    virtual ~Context() = default;

public:
    virtual void schedule(async::task_run_handle t) = 0;
    virtual unsigned long fps() const noexcept  = 0;
    virtual unsigned long fpsMax() const noexcept = 0;
    virtual void setFpsMax(unsigned long v) noexcept = 0;
};

VOLCANO_SIMULATION_END

#endif // VOLCANO_SIMULATION_CONTEXT_HPP
