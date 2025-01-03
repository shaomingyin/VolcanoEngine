//
//
#include <QOpenGLVersionFunctionsFactory>

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

thread_local OpenGLFunctions gl;

OpenGLFunctions::OpenGLFunctions()
    : context_(nullptr)
    , functions_(nullptr) {
}

void OpenGLFunctions::init(QOpenGLContext* context) {
    Q_ASSERT(context_ == nullptr);
    auto ctx = context ? context : QOpenGLContext::currentContext();
    Q_ASSERT(ctx->thread()->isCurrentThread());
    Q_ASSERT(functions_ == nullptr);
    functions_ = QOpenGLVersionFunctionsFactory::get<Functions>(ctx);
    if (functions_ != nullptr) {
        if (functions_->initializeOpenGLFunctions()) {
            context_ = ctx;
        } else {
            functions_ = nullptr;
        }
    }
}

VOLCANO_GRAPHICS_END
