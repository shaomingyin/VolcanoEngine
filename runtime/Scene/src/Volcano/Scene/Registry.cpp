//
//
#include <Volcano/Scene/Registry.hpp>

VOLCANO_SCENE_BEGIN

Registry::Registry()
    : global_(create()) {
    // on_construct<>()
}

VOLCANO_SCENE_END
