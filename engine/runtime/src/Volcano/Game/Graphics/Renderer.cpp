//
//
#include <memory>

#include <QQuickOpenGLUtils>

#include <Volcano/Game/Graphics/Camera.hpp>
#include <Volcano/Game/Graphics/Renderer.hpp>

VOLCANO_GAME_GRAPHICS_BEGIN

Renderer::Renderer(void):
    m_camera(nullptr),
    m_view(nullptr)
{
}

Renderer::~Renderer(void)
{
}

void Renderer::render(void)
{
    if (Q_UNLIKELY(m_camera == nullptr || m_view == nullptr))
        return;

    m_glRenderer.beginFrame();

    // TODO

    m_glRenderer.endFrame();

    m_camera->unlockView();

    QQuickOpenGLUtils::resetOpenGLState();
}

void Renderer::synchronize(QQuickFramebufferObject *item)
{
    if (Q_UNLIKELY(m_camera == nullptr)) {
        auto camera = qobject_cast<Camera *>(item);
        if (camera == nullptr)
            return;

        auto world = camera->world();
        if (world == nullptr)
            return;

        if (!m_glRenderer.init(item->width(), item->height()))
            return;

        auto &objects = world->objects();
        for (auto object: objects)
            addObject(object);

        m_camera = camera;
    }

    m_glRenderer.resize(QSize(item->width(), item->height()));
    m_view = m_camera->lockView();
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
        }
    }
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

VOLCANO_GAME_GRAPHICS_END
