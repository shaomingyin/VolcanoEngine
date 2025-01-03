//
//
#include <QScopeGuard>

#include <Volcano/Graphics/Pipeline.h>

VOLCANO_GRAPHICS_BEGIN

Pipeline::Pipeline(QOpenGLContext* gl_context) {
    enable(BoolAttribute::Clear);
    attr(ColorAttribute::ClearColor) = QColor::fromRgb(0.0f, 0.0f, 0.0f);
}

Pipeline::~Pipeline() {
}

void Pipeline::reset() {
}

void Pipeline::apply() {
    program_.bind();
}

void Pipeline::render(const View& view) {
    apply();

    gl()->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    gl()->glClear(GL_COLOR_BUFFER_BIT);
}

VOLCANO_GRAPHICS_END
