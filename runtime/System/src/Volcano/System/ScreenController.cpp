//
//
#include <QRunnable>
#include <QQuickGraphicsDevice>
#include <QQuickRenderTarget>

#include <Volcano/System/ScreenController.h>

VOLCANO_SYSTEM_BEGIN

ScreenController::ScreenController(World::Screen& screen, QObject* parent)
    : QObject(parent)
    , screen_(screen)
#ifdef VOLCANO_DEBUG
    , gl_context_(QOpenGLContext::currentContext())
#endif
    {
    Q_ASSERT(gl_context_ != nullptr);
    Q_ASSERT(thread() == screen_.thread());
    Q_ASSERT(QQuickWindow::graphicsApi() == QSGRendererInterface::OpenGL);
    if (screen_.isReady()) {
        onScreenStatusChanged(World::Screen::Status::Ready);
    }
    connect(&screen_, &World::Screen::statusChanged, this, &ScreenController::onScreenStatusChanged);
    connect(&screen_, &World::Screen::sizeChanged, this, &ScreenController::onScreenSizeChanged);
}

void ScreenController::update() {

}

void ScreenController::render(Graphics::View& view) {
    Q_ASSERT(gl_context_ == QOpenGLContext::currentContext());

    if (!window_) {
        return;
    }

    auto root_item = screen_.rootItem();
    if (root_item != nullptr) {
        root_item->setParentItem(window_->contentItem());
    }

    if (frame_buffer_->size() != screen_.size()) {
        frame_buffer_ = std::make_unique<QOpenGLFramebufferObject>(screen_.size());
        window_->setRenderTarget(QQuickRenderTarget::fromOpenGLTexture(frame_buffer_->texture(), frame_buffer_->size()));
    }

    Q_ASSERT(render_control_);

    render_control_->beginFrame();
    render_control_->polishItems();
    render_control_->sync();
    render_control_->render();
    render_control_->endFrame();
}

void ScreenController::onScreenStatusChanged(World::Screen::Status v) {
    frame_buffer_.reset();
    window_.reset();
    render_control_.reset();

    if (v != World::Screen::Status::Ready) {
        return;
    }

    render_control_ = std::make_unique<QQuickRenderControl>();
    window_ = std::make_unique<QQuickWindow>(render_control_.get());
    connect(window_.get(), &QQuickWindow::sceneGraphInitialized, this, &ScreenController::setupRenderTarget);

    window_->setGraphicsDevice(QQuickGraphicsDevice::fromOpenGLContext(gl_context_));
    if (!render_control_->initialize()) {
        warning() << "Failed to initialize screen rendering";
    }
}

void ScreenController::onScreenSizeChanged(const QSize& v) {
    if (window_) {
        window_->scheduleRenderJob(QRunnable::create([this] {
            setupRenderTarget();
        }), QQuickWindow::BeforeRenderingStage);
    }
}

void ScreenController::setupRenderTarget() {
    frame_buffer_ = std::make_unique<QOpenGLFramebufferObject>(screen_.size() * window_->devicePixelRatio());
    window_->setRenderTarget(QQuickRenderTarget::fromOpenGLTexture(frame_buffer_->texture(), frame_buffer_->size()));
}

VOLCANO_SYSTEM_END
