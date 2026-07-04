//
//
#include <Volcano/World/Inherent.h>

VOLCANO_WORLD_BEGIN

Inherent::Inherent() noexcept
    : flags_(FlagEnabled | FlagVisible) {
}

void to_json(nlohmann::json& json, const Inherent& v) {
    json = nlohmann::json::object();
    json["name"] = v.name_;
    json["enabled"] = v.isEnabled();
    json["visible"] = v.isVisible();
}

void from_json(const nlohmann::json& json, Inherent& v) {
    if (json.is_object()) {
        v.name_ = json["name"].get<std::string>();
        if (json["enabled"].get<bool>()) {
            v.enable();
        } else {
            v.disable();
        }
        if (json["visible"].get<bool>()) {
            v.show();
        } else {
            v.hide();
        }
    } else {
        throw std::invalid_argument("Invalid JSON format for Inherent");
    }
}

VOLCANO_WORLD_END
