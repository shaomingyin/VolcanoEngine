//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <QList>
#include <QAtomicInt>
#include <QOpenGLBuffer>

#include <Volcano/World/Scene.h>
#include <Volcano/World/Light.h>
#include <Volcano/World/DirectionalLight.h>
#include <Volcano/World/PointLight.h>
#include <Volcano/World/SpotLight.h>
#include <Volcano/World/Mesh.h>
#include <Volcano/World/Screen.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Context.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Pipeline.h>
#include <Volcano/Graphics/BufferAllocator.h>
#include <Volcano/Graphics/Mesh.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer: public Context {
    Q_OBJECT

public:
    Renderer(World::Scene& scene, QObject* parent = nullptr);
    ~Renderer() override;

public:
    void update(Duration elapsed);
    void render();
    Buffer* allocStaticVertexBuffer(GLsizei size) override;
    Buffer* allocStaticIndexBuffer(GLsizei size) override;

private slots:
    void onComponentAdded(World::Entity* entity, World::Component* component);
    void onComponentRemoved(World::Entity* entity, World::Component* component);

private:
    void onLightAdded(World::Entity* entity, World::Light* light);
    void onLightRemoved(World::Entity* entity, World::Light* light);

    void onMeshAdded(World::Entity* entity, World::Mesh* mesh);
    void onMeshRemoved(World::Entity* entity, World::Mesh* mesh);
    void loadNewMeshes();

    void onScreenAdded(World::Entity* entity, World::Screen* screen);
    void onScreenRemoved(World::Entity* entity, World::Screen* screen);

private:
    Pipeline forward_pipeline_;
    Pipeline deferred_pipeline_;
    BufferAllocator static_vertex_buffer_allocator_;
    BufferAllocator static_index_buffer_allocator_;

private:
    World::Scene& scene_;
    World::Light ambient_light_;
    //QHash<World::DirectionalLight*, World::Entity*> directional_lights_;
    //QHash<World::PointLight*, World::Entity*> point_lights_;
    //QHash<World::SpotLight*, World::Entity*> spot_lights_;
    //QList<Mesh> meshes_;
    std::array<View, 2> views_;
    QAtomicInt update_view_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
