//
//
#include <Volcano/World/Inherent.h>

VOLCANO_WORLD_BEGIN

Inherent::Inherent() noexcept
    : flags_(FlagEnabled | FlagVisible) {
}

Inherent::Inherent(std::string&& name, bool enabled, bool visible) noexcept
    : name_(std::move(name))
    , flags_((enabled ? FlagEnabled : 0) | (visible ? FlagVisible : 0)) {
}

VOLCANO_WORLD_END
