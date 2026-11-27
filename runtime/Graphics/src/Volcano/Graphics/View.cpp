//
//
#include <Volcano/Graphics/View.h>

VOLCANO_GRAPHICS_BEGIN

View::View(int w, int h) {
}

View::View(const QRectF& viewport, const QMatrix4x4& viewMatrix, const QMatrix4x4& projectionMatrix)
    : viewport_(viewport)
    , view_matrix_(viewMatrix)
    , projection_matrix_(projectionMatrix) {
}

View::~View() {
}

void View::reset() {
}

VOLCANO_GRAPHICS_END
