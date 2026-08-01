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
}

void Frame::commit(int x, int y, int width, int height) const noexcept {
    glViewport(x, y, width, height);

    if (get(Bool::Clear)) {
        auto& color = get(Vector4f::ClearColor);
        glClearColor(color.x() / 255.0f, color.y() / 255.0f, color.z() / 255.0f, color.w() / 255.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

VOLCANO_GRAPHICS_END
