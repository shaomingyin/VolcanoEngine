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
    m_view(nullptr),
    m_gl(nullptr),
    m_gBuffer(nullptr)
{
}

Renderer::~Renderer(void)
{
    if (m_gBuffer != nullptr)
        releaseGBuffer();
}

bool Renderer::init(void)
{
    Q_ASSERT(m_gl == nullptr);

    m_gl = glFunctions();
    if (m_gl == nullptr)
        return false;

    QScopeGuard glFuncsGuard([this] { m_gl = nullptr; });

    updateGBuffer();
    if (m_gBuffer == nullptr)
        return false;

    QScopeGuard gBufferGuard([this] { releaseGBuffer(); });

    // TODO init resources...

    QObject::connect(&m_camera, &Camera::worldChanged, [this] (World *world) {
        attachWorld(world);
    });

    auto world = m_camera.world();
    if (world == nullptr)
        return false;

    attachWorld(world);

    gBufferGuard.dismiss();
    glFuncsGuard.dismiss();

    return true;
}

void Renderer::reset(void)
{
}

void Renderer::updateGBuffer(void)
{
    if (Q_LIKELY(m_gBuffer != nullptr)) {
        if (Q_LIKELY(m_gBuffer->size() == m_camera.size()))
            return;
        releaseGBuffer();
    }

    m_gBuffer = new QOpenGLFramebufferObject(m_camera.width(), m_camera.height());
}

void Renderer::releaseGBuffer(void)
{
    Q_ASSERT(m_gl != nullptr);
    Q_ASSERT(m_gBuffer != nullptr);

    // TODO

    m_gBuffer = nullptr;
}

void Renderer::render(void)
{
    Q_ASSERT(m_gl != nullptr);

    if (Q_UNLIKELY(m_view == nullptr))
        return;

    if (m_camera.isClear()) {
        auto &color = m_camera.clearColor();
        m_gl->glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
        m_gl->glClear(GL_COLOR_BUFFER_BIT);
    }

    // TODO

    m_camera.unlockView();

    QQuickOpenGLUtils::resetOpenGLState();
}

void Renderer::synchronize(QQuickFramebufferObject *item)
{
    Q_ASSERT(m_gBuffer != nullptr);
    Q_ASSERT(static_cast<QQuickFramebufferObject *>(&m_camera) == item);

    updateGBuffer();

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
