//
//
#ifndef VOLCANO_GAME_ENTITY_HPP
#define VOLCANO_GAME_ENTITY_HPP

#include <memory>
#include <vector>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Component.hpp>
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

class Entity: public Object {
public:
	using Components = std::vector<std::unique_ptr<Component>>;

public:
	Entity();
	virtual ~Entity();

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

	Transform& transform() noexcept {
		return transform_;
	}

	const Transform& transform() const noexcept {
		return transform_;
	}

	Components& components() noexcept {
		return components_;
	}

    const Components& components() const noexcept {
        return components_;
    }

	void update(Clock::duration elapsed, Scheduler& scheduler) override;
	Graphics::VisibleSet buildVisibleSet(Scheduler& scheduler) const override;

private:
    enum {
        FlagEnabled = 0x1,
        FlagVisible = 0x2
    };

    int flags_;
    Transform transform_;
    Components components_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ENTITY_HPP
