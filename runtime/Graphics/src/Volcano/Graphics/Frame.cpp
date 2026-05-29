//
//
#include <Volcano/Graphics/Frame.h>

VOLCANO_GRAPHICS_BEGIN

Frame::Frame() {
    reset();
}

void Frame::reset() noexcept {
    get(Bool::Clear) = true;
    get(Affine3f::View) = Eigen::Affine3f::Identity();
    lights_.clear();
    directional_lights_.clear();
    point_lights_.clear();
    spot_lights_.clear();
}

void Frame::commit(int x, int y, int width, int height) const noexcept {
    glViewport(x, y, width, height);

    if (get(Bool::Clear)) {
        auto& color = get(Color::Clear);
        glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

VOLCANO_GRAPHICS_END
