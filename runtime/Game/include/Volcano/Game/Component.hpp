//
//
#ifndef VOLCANO_GAME_COMPONENT_HPP
#define VOLCANO_GAME_COMPONENT_HPP

#include <cassert>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Context.hpp>

VOLCANO_GAME_BEGIN

class ComponentBase {
    RTTR_ENABLE()

public:
    ComponentBase()
        : context_(nullptr) {
    }

    virtual ~ComponentBase() = default;

public:
    Context& context() noexcept {
        assert(context_ != nullptr);
        return *context_;
    }

    const Context& context() const noexcept {
        assert(context_ != nullptr);
        return *context_;
    }

	void setContext(Context& context) noexcept {
        assert(context_ == nullptr);
        context_ = &context;
    }

private:
    Context* context_;
};

template <typename T>
concept Component = std::is_base_of_v<ComponentBase, T>;

VOLCANO_GAME_END

#endif // VOLCANO_GAME_COMPONENT_HPP
