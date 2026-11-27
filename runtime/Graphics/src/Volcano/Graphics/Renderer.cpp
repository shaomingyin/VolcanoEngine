//
//
#include <algorithm>
#include <stdexcept>
#include <utility>

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QScopeGuard>
#include <QThread>
#include <QVariant>

#include <Volcano/Graphics/Model.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/World/Camera.h>
#include <Volcano/World/Model.h>
#include <Volcano/World/Scene.h>

VOLCANO_GRAPHICS_BEGIN

namespace {
constexpr char modelBinding[] = "volcano.graphics.model";
}

Renderer::Renderer(World::Scene& scene, QObject* parent)
    : Context(parent)
    , scene_(scene) {
    Q_ASSERT(scene_.thread() == thread());
    connect(&scene_, &World::Scene::componentAdded, this, &Renderer::attachModel);
    connect(&scene_, &World::Scene::componentRemoved, this, &Renderer::detachModel);

    auto rollback = qScopeGuard([this] { clearModelBindings(); });
    for (auto* entity: scene_.entities()) {
        for (auto* component: entity->components()) {
            attachModel(entity, component);
        }
    }
    rollback.dismiss();
}

Renderer::~Renderer() {
    Q_ASSERT(!renderState_ || (&renderState_->context == QOpenGLContext::currentContext()
                               && renderState_->thread == QThread::currentThread()));
    renderState_.reset();
    clearModelBindings();
}

Renderer::RenderState::RenderState(QOpenGLContext& context)
    : context(context)
    , thread(QThread::currentThread())
    , vertexAllocator(QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::StaticDraw)
    , indexAllocator(QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::StaticDraw) {
}

void Renderer::clearModelBindings() {
    disconnect(&scene_, nullptr, this, nullptr);
    for (auto& instance: models_) {
        clearModelBinding(instance);
    }
}

void Renderer::attachModel(World::Entity* entity, QObject* component) {
    auto* sceneModel = qobject_cast<World::Model*>(component);
    // shortcut: one renderer per model component, add per-renderer bindings for shared scenes
    if (!sceneModel || sceneModel->property(modelBinding).isValid()) {
        return;
    }

    models_.push_back({entity, sceneModel});
    sceneModel->setProperty(modelBinding, QVariant::fromValue<QObject*>(this));
}

void Renderer::detachModel(World::Entity* entity, QObject* component) {
    if (!component) {
        return;
    }
    const auto instance = std::ranges::find_if(models_, [entity, component](const ModelInstance& item) {
        return item.entity == entity && item.component == component;
    });
    if (instance != models_.end()) {
        clearModelBinding(*instance);
    }
}

void Renderer::clearModelBinding(ModelInstance& instance) {
    const auto component = std::exchange(instance.component, nullptr);
    if (component && component->property(modelBinding).value<QObject*>() == this) {
        component->setProperty(modelBinding, QVariant{});
    }
}

void Renderer::update(const World::Camera& camera, const QRect& viewport) {
    Q_ASSERT(scene_.thread() == QThread::currentThread());
    Q_ASSERT(camera.thread() == scene_.thread());
    std::erase_if(models_, [this](ModelInstance& instance) {
        if (!instance.entity) {
            clearModelBinding(instance);
        }
        return !instance.component;
    });

    auto frame = std::make_shared<Frame>();
    frame->view = View(viewport, camera.viewMatrix(), camera.projectionMatrix());
    frame->models.reserve(models_.size());
    for (const auto& instance: models_) {
        if (instance.entity->isEnabled()) {
            frame->models.push_back({instance.component->source(), instance.entity->transform()->toMatrix()});
        }
    }
    std::shared_ptr<const Frame> published = std::move(frame);
    const std::lock_guard lock(frameMutex_);
    frame_.swap(published);
}

Renderer::RenderState& Renderer::renderState() {
    auto* context = QOpenGLContext::currentContext();
    if (!context || (renderState_ && (&renderState_->context != context
                                    || renderState_->thread != QThread::currentThread()))) {
        throw std::logic_error("Renderer requires its OpenGL context to be current");
    }
    if (!renderState_) {
        renderState_.emplace(*context);
    }
    return *renderState_;
}

void Renderer::render() {
    std::shared_ptr<const Frame> frame;
    {
        const std::lock_guard lock(frameMutex_);
        frame = frame_;
    }
    if (!frame) {
        return;
    }

    auto& state = renderState();
    const auto viewport = frame->view.viewport().toRect();
    state.context.functions()->glViewport(viewport.x(), viewport.y(), viewport.width(), viewport.height());
    // shortcut: reuse draw slots by frame order, add stable identities when models own instance-specific GPU state
    state.models.resize(frame->models.size());
    for (std::size_t i = 0; i < frame->models.size(); ++i) {
        auto& model = state.models[i];
        if (!model) {
            model = std::make_unique<Model>(*this);
        }
        const auto& data = frame->models[i];
        model->setSceneState(data.source, data.worldMatrix);
        model->render();
    }
}

void Renderer::releaseRenderResources() {
    if (renderState_) {
        renderState();
        renderState_.reset();
    }
}

Buffer* Renderer::allocStaticVertexBuffer(GLsizei size) {
    return renderState().vertexAllocator.allocBuffer(size);
}

Buffer* Renderer::allocStaticIndexBuffer(GLsizei size) {
    return renderState().indexAllocator.allocBuffer(size);
}

VOLCANO_GRAPHICS_END
