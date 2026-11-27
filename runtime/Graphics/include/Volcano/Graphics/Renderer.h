//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <memory>
#include <mutex>
#include <optional>
#include <vector>

#include <QPointer>
#include <QRect>
#include <QUrl>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Context.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/BufferAllocator.h>

VOLCANO_BEGIN
namespace World {
class Camera;
class Entity;
class Model;
class Scene;
}
VOLCANO_END

VOLCANO_GRAPHICS_BEGIN

class Model;

class Renderer: public Context {
    Q_OBJECT

public:
    explicit Renderer(World::Scene& scene, QObject* parent = nullptr);
    ~Renderer() override;

public:
    // Update on the scene thread. Render and release resources on the GL context's thread.
    void update(const World::Camera& camera, const QRect& viewport);
    void render();
    // Release here before destroying the renderer on a different thread.
    void releaseRenderResources();
    Buffer* allocStaticVertexBuffer(GLsizei size) override;
    Buffer* allocStaticIndexBuffer(GLsizei size) override;

private:
    struct ModelInstance {
        QPointer<World::Entity> entity;
        QPointer<World::Model> component;
    };

    struct FrameModel {
        QUrl source;
        QMatrix4x4 worldMatrix;
    };

    struct Frame {
        View view;
        std::vector<FrameModel> models;
    };

    struct RenderState {
        explicit RenderState(QOpenGLContext& context);

        QOpenGLContext& context;
        QThread* thread;
        BufferAllocator vertexAllocator;
        BufferAllocator indexAllocator;
        std::vector<std::unique_ptr<Model>> models;
    };

    void attachModel(World::Entity* entity, QObject* component);
    void detachModel(World::Entity* entity, QObject* component);
    void clearModelBinding(ModelInstance& instance);
    void clearModelBindings();
    RenderState& renderState();

    World::Scene& scene_;
    std::vector<ModelInstance> models_;
    std::mutex frameMutex_;
    std::shared_ptr<const Frame> frame_;
    std::optional<RenderState> renderState_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
