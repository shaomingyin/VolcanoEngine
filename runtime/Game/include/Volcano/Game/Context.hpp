//
//
#ifndef VOLCANO_GAME_CONTEXT_HPP
#define VOLCANO_GAME_CONTEXT_HPP

#include <chrono>

#include <async++.h>

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Context {
public:
    using Clock = std::chrono::steady_clock;

public:
    Context() = default;
    virtual ~Context();

public:
    void makeCurrent();

    inline static Context* current() {
		return current_;
    }

public:
    virtual void schedule(async::task_run_handle t) = 0;

private:
    static Context* current_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CONTEXT_HPP
