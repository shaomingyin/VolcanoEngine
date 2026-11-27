//
//
#include <Volcano/Editor/SceneEditorViewport.h>
#include <Volcano/World/Camera.h>

VOLCANO_EDITOR_BEGIN

SceneEditorViewport::SceneEditorViewport(QWidget* parent)
    : QOpenGLWidget(parent) {
    setWindowTitle("World");
}

SceneEditorViewport::~SceneEditorViewport() {
    if (auto* glContext = context()) {
        disconnect(glContext, &QOpenGLContext::aboutToBeDestroyed, this, &SceneEditorViewport::releaseRenderer);
    }
    setScene(nullptr);
}

void SceneEditorViewport::releaseRenderer() {
    if (renderer_) {
        makeCurrent();
        renderer_->releaseRenderResources();
        renderer_.reset();
        doneCurrent();
    }
}

void SceneEditorViewport::setScene(World::Scene* scene) {
    if (scene_ == scene) {
        return;
    }
    releaseRenderer();
    if (scene_) {
        disconnect(scene_, &World::Scene::aboutToBeDestroyed, this, nullptr);
    }
    scene_ = scene;
    camera_.clear();
    if (scene_) {
        connect(scene_, &World::Scene::aboutToBeDestroyed, this, [this] { setScene(nullptr); });
    }
    update();
}

void SceneEditorViewport::initializeGL() {
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &SceneEditorViewport::releaseRenderer);
}

void SceneEditorViewport::setCamera(World::Camera* camera) {
    camera_ = camera;
    update();
}

void SceneEditorViewport::paintGL() {
    if (!scene_) {
        return;
    }
    if (!renderer_) {
        renderer_ = std::make_unique<Graphics::Renderer>(*scene_);
    }
    if (!camera_) {
        return;
    }

    const auto scale = devicePixelRatioF();
    renderer_->update(*camera_, QRect(0, 0, qRound(width() * scale), qRound(height() * scale)));
    renderer_->render();
}

VOLCANO_EDITOR_END
