//
//
#ifndef VOLCANO_WORLD_INHERENT_H
#define VOLCANO_WORLD_INHERENT_H

#include <string>
#include <string_view>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Inherent {
public:
    constexpr Inherent() noexcept
        : flags_(FlagEnabled | FlagVisible) {
    }

    constexpr explicit Inherent(std::string_view name, bool enabled = true, bool visible = true)
        : name_(name)
        , flags_((enabled ? FlagEnabled : 0) | (visible ? FlagVisible : 0)) {
    }

    constexpr Inherent(const Inherent&) = default;
    constexpr Inherent(Inherent&&) = default;

public:
    constexpr Inherent& operator=(const Inherent&) = default;
    constexpr Inherent& operator=(Inherent&&) = default;

    constexpr const std::string& name() const noexcept {
        return name_;
    }

    constexpr void setName(std::string_view value) {
        name_.assign(value);
    }

    constexpr bool isEnabled() const noexcept {
        return (flags_ & FlagEnabled) != 0;
    }

    constexpr void enable() noexcept {
        flags_ |= FlagEnabled;
    }

    constexpr void disable() noexcept {
        flags_ &= ~FlagEnabled;
    }

    constexpr bool isVisible() const noexcept {
        return (flags_ & FlagVisible) != 0;
    }

    constexpr void show() noexcept {
        flags_ |= FlagVisible;
    }

    constexpr void hide() noexcept {
        flags_ &= ~FlagVisible;
    }

private:
    enum {
        FlagEnabled = 0x1,
        FlagVisible = 0x2
    };

    std::string name_;
    int flags_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_INHERENT_H
