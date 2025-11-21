//
//
#ifndef VOLCANO_UTILS_VIEWBUILDER_HPP
#define VOLCANO_UTILS_VIEWBUILDER_HPP

#include <Volcano/Simulation/World.hpp>
#include <Volcano/Graphics/View.hpp>
#include <Volcano/Utils/Common.hpp>

VOLCANO_UTILS_BEGIN

class ViewBuilder {
public:
    ViewBuilder(const Simulation::World& world);
    virtual ~ViewBuilder() = default;

public:
    const Simulation::World& world() const noexcept {
        return world_;
    }

    virtual void build(Graphics::View& view);

private:
    const Simulation::World& world_;
};

VOLCANO_UTILS_END

#endif // VOLCANO_UTILS_VIEWBUILDER_HPP
