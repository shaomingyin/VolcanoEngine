//
//
#include <Volcano/Graphics/Frame.h>

VOLCANO_GRAPHICS_BEGIN

Frame::Frame() {
    set(Bool::Clear, true);
    set(Affine3f::View, Eigen::Affine3f::Identity());
}

void Frame::draw() const noexcept {

}

VOLCANO_GRAPHICS_END
