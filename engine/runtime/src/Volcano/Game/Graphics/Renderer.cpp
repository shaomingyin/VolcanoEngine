//
//
#include <memory>

#include <QScopeGuard>
#include <QObject>
#include <QQuickOpenGLUtils>

#include <Volcano/Game/Graphics/Camera.hpp>
#include <Volcano/Game/Graphics/Renderer.hpp>

VOLCANO_GAME_GRAPHICS_BEGIN

Renderer::Renderer(Camera &camera):
    m_camera(camera),
    m_world(nullptr),
    m_view(nullptr)
{
}

Renderer::~Renderer(void)
{
}

bool Renderer::init(void)
{
    // TODO init resources...

    QObject::connect(&m_camera, &Camera::worldChanged, [this] (World *world) {
        attachWorld(world);
    });

    auto world = m_camera.world();
    if (world == nullptr)
        return false;

    attachWorld(world);

    return true;
}

void Renderer::reset(void)
{
}

void Renderer::render(void)
{
    if (Q_UNLIKELY(m_view == nullptr))
        return;

    if (m_camera.isClear()) {
        // TODO
        m_glRenderer.setClearColor(m_camera.clearColor());
        m_glRenderer.clear();
    }

    m_glRenderer.beginFrame();

    // TODO

    m_glRenderer.endFrame();

    m_camera.unlockView();

    QQuickOpenGLUtils::resetOpenGLState();
}

void Renderer::synchronize(QQuickFramebufferObject *item)
{
    Q_ASSERT(static_cast<QQuickFramebufferObject *>(&m_camera) == item);

    m_view = m_camera.lockView();
}

void Renderer::addObject(Object *object)
{
    auto entity = qobject_cast<Entity *>(object);
    if (entity != nullptr) {
        addEntity(entity);
        return;
    }

    auto spotLight = qobject_cast<SpotLight *>(object);
    if (spotLight != nullptr) {
        addSpotLight(spotLight);
        return;
    }

    auto directionalLight = qobject_cast<DirectionalLight *>(object);
    if (directionalLight != nullptr) {
        addDirectionalLight(directionalLight);
        return;
    }

    auto pointLight = qobject_cast<PointLight *>(object);
    if (pointLight != nullptr) {
        addPointLight(pointLight);
        return;
    }
}

void Renderer::addEntity(Entity *entity)
{
    auto components = entity->components();
    for (auto component: components) {
        auto mesh = qobject_cast<Mesh *>(component);
        if (mesh != nullptr) {
            addMesh(mesh);
        }
    }
}

void Renderer::addMesh(Mesh *mesh)
{
}

void Renderer::addDirectionalLight(DirectionalLight *directionalLight)
{

}

void Renderer::addPointLight(PointLight *pointLight)
{

}

void Renderer::addSpotLight(SpotLight *spotLight)
{

}

void Renderer::attachWorld(World *world)
{
    if (m_world == world)
        return;

    reset();

    m_world = world;
    if (m_world == nullptr)
        return;

    auto &objects = world->objects();
    for (auto object: objects)
        addObject(object);
}

VOLCANO_GAME_GRAPHICS_END
