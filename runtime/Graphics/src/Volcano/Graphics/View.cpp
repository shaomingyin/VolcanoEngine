//
//
#include <Volcano/Graphics/View.h>

VOLCANO_GRAPHICS_BEGIN

View::View() {
    set(Bool::Clear, true);
    set(Affine3f::View, Eigen::Affine3f::Identity());
}

VOLCANO_GRAPHICS_END
