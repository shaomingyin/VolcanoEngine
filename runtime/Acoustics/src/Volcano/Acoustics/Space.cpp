//
//
#include <Volcano/Acoustics/Space.h>

VOLCANO_ACOUSTICS_BEGIN

Space::Space(entt::registry& registry)
	: registry_(registry) {
}

void Space::frame(Duration elapsed) {
}

VOLCANO_ACOUSTICS_END
