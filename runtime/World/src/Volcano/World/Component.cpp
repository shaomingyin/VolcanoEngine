//
//
#include <Volcano/World/Component.h>

VOLCANO_WORLD_BEGIN

Component::Component() {
}

Component::~Component() {
}

void Component::loadFromJson(const nlohmann::json& j) {
}

void Component::saveToJson(nlohmann::json& j) {
}

VOLCANO_WORLD_END
