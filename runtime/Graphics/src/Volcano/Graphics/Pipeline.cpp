//
//
#include <QScopeGuard>
#include <QOpenGLVersionFunctionsFactory>

#include <Volcano/Graphics/Pipeline.h>

VOLCANO_GRAPHICS_BEGIN

Pipeline::Pipeline(QOpenGLContext* gl_context)
    : gl_(QOpenGLVersionFunctionsFactory::get<OpenGLFunctions>(gl_context)) {
    Q_ASSERT(gl_ != nullptr);
    enable(BoolAttribute::Clear);
    attr(ColorAttribute::ClearColor) = QColor::fromRgb(0.0f, 0.0f, 0.0f);
}

Pipeline::~Pipeline() {
}

void Pipeline::reset() {
}

void Pipeline::apply() {
    Q_ASSERT(gl_ != nullptr);

    program_.bind();
}

void Pipeline::render(const View& view) {
    Q_ASSERT(gl_ != nullptr);

    apply();

    gl_->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    gl_->glClear(GL_COLOR_BUFFER_BIT);
}

VOLCANO_GRAPHICS_END
