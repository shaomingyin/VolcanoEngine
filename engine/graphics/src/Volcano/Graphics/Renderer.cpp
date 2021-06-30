//
//
#include <memory>

#include <QScopeGuard>
#include <QObject>
#include <QQuickOpenGLUtils>

#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Graphics/Renderer.hpp>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(Camera &camera):
    m_camera(camera),
    m_gameWorld(nullptr),
    m_view(nullptr)
{
}

Renderer::~Renderer(void)
{
}

bool Renderer::init(void)
{
    // TODO init resources...

    QObject::connect(&m_camera, &Camera::gameWorldChanged, [this] (Game::World *gameWorld) {
        attachGameWorld(gameWorld);
    });

    auto gameWorld = m_camera.gameWorld();
    if (gameWorld == nullptr)
        return false;

    attachGameWorld(gameWorld);

    return true;
}

void Renderer::reset(void)
{
}

void Renderer::render(void)
{
    if (Q_UNLIKELY(m_view == nullptr))
        return;

    // TODO

    m_camera.unlockView();

    QQuickOpenGLUtils::resetOpenGLState();
}

void Renderer::synchronize(QQuickFramebufferObject *item)
{
    Q_ASSERT(static_cast<QQuickFramebufferObject *>(&m_camera) == item);

    m_view = m_camera.lockView();
}

void Renderer::addObject(Game::Object *object)
{
    auto entity = qobject_cast<Game::Entity *>(object);
    if (entity != nullptr) {
        addEntity(entity);
        return;
    }

    auto spotLight = qobject_cast<Game::SpotLight *>(object);
    if (spotLight != nullptr) {
        addSpotLight(spotLight);
        return;
    }

    auto directionalLight = qobject_cast<Game::DirectionalLight *>(object);
    if (directionalLight != nullptr) {
        addDirectionalLight(directionalLight);
        return;
    }

    auto pointLight = qobject_cast<Game::PointLight *>(object);
    if (pointLight != nullptr) {
        addPointLight(pointLight);
        return;
    }
}

void Renderer::addEntity(Game::Entity *entity)
{
    auto components = entity->components();
    for (auto component: components) {
        auto mesh = qobject_cast<Game::Mesh *>(component);
        if (mesh != nullptr) {
            addMesh(mesh);
        }
    }
}

void Renderer::addMesh(Game::Mesh *mesh)
{
}

void Renderer::addDirectionalLight(Game::DirectionalLight *directionalLight)
{

}

void Renderer::addPointLight(Game::PointLight *pointLight)
{

}

void Renderer::addSpotLight(Game::SpotLight *spotLight)
{

}

void Renderer::attachGameWorld(Game::World *gameWorld)
{
    if (m_gameWorld == gameWorld)
        return;

    reset();

    m_gameWorld = gameWorld;
    if (m_gameWorld == nullptr)
        return;

    auto &objects = gameWorld->objects();
    for (auto object: objects)
        addObject(object);
}

VOLCANO_GRAPHICS_END
