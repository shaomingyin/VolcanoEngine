//
//
#include <QThread>
#include <QScopeGuard>
#include <QOpenGLContext>

#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(World::Scene& scene, QObject* parent)
    : Context(parent)
    , static_vertex_buffer_allocator_(QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::StaticDraw)
    , static_index_buffer_allocator_(QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::StaticDraw)
    , scene_(scene)
    , update_view_(0) {
    auto gl_context = QOpenGLContext::currentContext();
    Q_ASSERT(gl_context != nullptr);
    Q_ASSERT(gl_context->thread()->isCurrentThread());
    for (auto e: scene_.entities()) {
        for (auto c: e->components()) {
            onComponentAdded(e, c);
        }
    }
    connect(&scene_, &World::Scene::componentAdded, this, &Renderer::onComponentAdded, Qt::QueuedConnection);
    connect(&scene_, &World::Scene::componentAdded, this, &Renderer::onComponentRemoved, Qt::QueuedConnection);
}

Renderer::~Renderer() {
}

void Renderer::update(Duration elapsed) {
    loadNewMeshes();
    View& view = views_[update_view_];

    if (thread()->isCurrentThread()) {
        Q_ASSERT(QOpenGLContext::currentContext() != nullptr);
        Q_ASSERT(QOpenGLContext::currentContext()->thread()->isCurrentThread());
    } else {
    }
}

void Renderer::render() {
    Q_ASSERT(thread()->isCurrentThread());
    Q_ASSERT(QOpenGLContext::currentContext() != nullptr);
    Q_ASSERT(QOpenGLContext::currentContext()->thread()->isCurrentThread());

    // render screens

    // defered lighting
    // deferred_pipeline_.render(views_[0]);

    // render transparency objects
    // forward_pipeline_.render(views_[0]);
}

Buffer* Renderer::allocStaticVertexBuffer(GLsizei size) {
    Q_ASSERT(thread()->isCurrentThread());
    return static_vertex_buffer_allocator_.allocBuffer(size);
}

Buffer* Renderer::allocStaticIndexBuffer(GLsizei size) {
    Q_ASSERT(thread()->isCurrentThread());
    return static_index_buffer_allocator_.allocBuffer(size);
}

void Renderer::onComponentAdded(World::Entity* entity, World::Component* component) {
    auto light = qobject_cast<World::Light*>(component);
    if (light != nullptr) {
        onLightAdded(entity, light);
        return;
    }
    auto mesh = qobject_cast<World::Mesh*>(component);
    if (mesh != nullptr) {
        onMeshAdded(entity, mesh);
        return;
    }
    auto screen = qobject_cast<World::Screen*>(component);
    if (screen != nullptr) {
        onScreenAdded(entity, screen);
        return;
    }
}

void Renderer::onComponentRemoved(World::Entity* entity, World::Component* component) {
    auto light = qobject_cast<World::Light*>(component);
    if (light != nullptr) {
        onLightRemoved(entity, light);
        return;
    }
    auto mesh = qobject_cast<World::Mesh*>(component);
    if (mesh != nullptr) {
        onMeshRemoved(entity, mesh);
        return;
    }
    auto screen = qobject_cast<World::Screen*>(component);
    if (screen != nullptr) {
        onScreenRemoved(entity, screen);
        return;
    }
}

void Renderer::onLightAdded(World::Entity* entity, World::Light* light) {
    auto point_light = qobject_cast<World::PointLight*>(light);
    if (point_light != nullptr) {
        //point_lights_[point_light] = entity;
        return;
    }
    auto spot_light = qobject_cast<World::SpotLight*>(light);
    if (spot_light != nullptr) {
        //spot_lights_[spot_light] = entity;
        return;
    }
    auto directional_light = qobject_cast<World::DirectionalLight*>(light);
    if (directional_light != nullptr) {
        //directional_lights_[directional_light] = entity;
        return;
    }
    //ambient_light_.color() += light->color();
}

void Renderer::onLightRemoved(World::Entity* entity, World::Light* light) {
    auto point_light = qobject_cast<World::PointLight*>(light);
    if (point_light != nullptr) {
        //point_lights_.remove(point_light);
        return;
    }
    auto spot_light = qobject_cast<World::SpotLight*>(light);
    if (spot_light != nullptr) {
        //spot_lights_.remove(spot_light);
        return;
    }
    auto directional_light = qobject_cast<World::DirectionalLight*>(light);
    if (directional_light != nullptr) {
        //directional_lights_.remove(directional_light);
        return;
    }
}

void Renderer::onMeshAdded(World::Entity* entity, World::Mesh* mesh) {
    meshes_.emplaceBack(*this, entity, mesh);
}

void Renderer::onMeshRemoved(World::Entity* entity, World::Mesh* mesh) {
    //meshes_.removeIf()
}

void Renderer::loadNewMeshes() {
    Q_ASSERT(QOpenGLContext::currentContext() != nullptr);
    Q_ASSERT(QOpenGLContext::currentContext()->thread()->isCurrentThread());
    for (auto it = new_meshes_.begin(); it != new_meshes_.end(); ++it) {
    }
    new_meshes_.clear();
}

void Renderer::onScreenAdded(World::Entity* entity, World::Screen* screen) {
    screens_[screen] = entity;
}

void Renderer::onScreenRemoved(World::Entity* entity, World::Screen* screen) {
    screens_.remove(screen);
}

VOLCANO_GRAPHICS_END
