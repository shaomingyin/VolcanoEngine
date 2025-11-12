//
//
#include <Volcano/Scene/Registry.h>

VOLCANO_SCENE_BEGIN

Registry::Registry()
    : global_(create()) {
    // on_construct<>()
}

VOLCANO_SCENE_END
