//
//
#ifndef VOLCANO_GAME_OBJECT_HPP
#define VOLCANO_GAME_OBJECT_HPP

#include <async++.h>

#include <Volcano/Graphics/VisibleSet.hpp>
#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Object {
public:
	struct Scheduler {
		virtual void schedule(async::task_run_handle t) = 0;
	};

public:
	Object() = default;
	virtual ~Object() = default;

public:
	bool isEnabled() const noexcept {
		return (flags_ & FlagEnabled);
	}

	void enable() noexcept {
		flags_ |= FlagEnabled;
	}

	void disable() noexcept {
		flags_ &= ~FlagEnabled;
	}

	bool isVisible() const noexcept {
		return (flags_ & FlagVisible);
	}

	void show() noexcept {
		flags_ |= FlagVisible;
	}

	void hide() noexcept {
		flags_ &= ~FlagVisible;
	}

	void tick(Clock::duration elapsed, Scheduler& scheduler) noexcept {
		if (flags_ & FlagEnabled) {
			update(elapsed, scheduler);
		}
	}

	Graphics::VisibleSet buildVisibleSet(Scheduler& scheduler) const noexcept {
		Graphics::VisibleSet visible_set;
		if (flags_ & FlagVisible) {
			visible_set = makeVisibleSet(scheduler);
		}
		return visible_set;
	}

protected:
	virtual void update(Clock::duration elapsed, Scheduler& scheduler) = 0;
	virtual Graphics::VisibleSet makeVisibleSet(Scheduler& scheduler) const = 0;

private:
	enum {
		FlagEnabled = 0x1,
		FlagVisible = 0x2
	};

	int flags_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_OBJECT_HPP
