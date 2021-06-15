//
//
#include <memory>

#include <QQuickOpenGLUtils>

#include <Volcano/Graphics/WorldView.hpp>
#include <Volcano/Graphics/WorldRenderer.hpp>

VOLCANO_GRAPHICS_BEGIN

WorldRenderer::WorldRenderer(void):
    m_glRenderer(nullptr),
    m_camera(nullptr)
{
}

WorldRenderer::~WorldRenderer(void)
{
}

void WorldRenderer::render(void)
{
    if (Q_UNLIKELY(m_glRenderer == nullptr))
        return;

    m_glRenderer->beginFrame();

    // TODO

    m_glRenderer->endFrame();

    QQuickOpenGLUtils::resetOpenGLState();
}

void WorldRenderer::synchronize(QQuickFramebufferObject *item)
{
    if (Q_LIKELY(m_glRenderer != nullptr)) {
        buildVisibleSet();
        return;
    }

    auto view = qobject_cast<WorldView *>(item);
    if (Q_UNLIKELY(view == nullptr))
        return;

    m_glRenderer = new OpenGL::Renderer();
    if (m_glRenderer == nullptr)
        return;

    attachCamera(view->camera());
    buildVisibleSet();
}

void WorldRenderer::attachCamera(Camera *camera)
{
    if (m_camera == camera)
        return;

    if (camera != nullptr) {
    }

    m_camera = camera;
}

void WorldRenderer::addObject(Object *object)
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

void WorldRenderer::addEntity(Entity *entity)
{
}

void WorldRenderer::addDirectionalLight(DirectionalLight *directionalLight)
{

}

void WorldRenderer::addPointLight(PointLight *pointLight)
{

}

void WorldRenderer::addSpotLight(SpotLight *spotLight)
{

}

void WorldRenderer::buildVisibleSet(void)
{
    // TODO clear visible set

    if (Q_UNLIKELY(m_camera == nullptr))
        return;

    QMatrix4x4 projectMatrix;
    m_camera->calcProjectMatrix(projectMatrix);

    // TODO
}

VOLCANO_GRAPHICS_END
