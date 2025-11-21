//
//
#ifndef VOLCANO_WORLD_COMPONENT_HPP
#define VOLCANO_WORLD_COMPONENT_HPP

#include <Volcano/World/Common.hpp>
#include <Volcano/World/Context.hpp>

VOLCANO_WORLD_BEGIN

class ComponentBase {
    RTTR_ENABLE()

public:
    ComponentBase(Context& context)
        : context_(context) {
    }

    virtual ~ComponentBase() = default;

public:
    Context& context() noexcept {
        return context_;
    }

    const Context& context() const noexcept {
        return context_;
    }

private:
    Context& context_;
};

template <typename T>
concept Component = std::is_base_of_v<ComponentBase, T>;

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_COMPONENT_HPP
