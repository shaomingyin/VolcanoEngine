//
//
#ifndef VOLCANO_GAME_COMPONENT_HPP
#define VOLCANO_GAME_COMPONENT_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Transform.hpp>
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

class Component: public Object {
public:
    Component();
    virtual ~Component();

public:
    Transform& transform() noexcept {
        return transform_;
    }

    const Transform& transform() const noexcept {
        return transform_;
    }

    void update(Clock::duration elapsed, Scheduler& scheduler) override;
    Graphics::VisibleSet buildVisibleSet(Scheduler& scheduler) const override;

private:
    Transform transform_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_COMPONENT_HPP
