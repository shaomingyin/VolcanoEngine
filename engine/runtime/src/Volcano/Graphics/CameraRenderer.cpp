//
//
#include <memory>

#include <QQuickOpenGLUtils>

#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Graphics/CameraRenderer.hpp>

VOLCANO_GRAPHICS_BEGIN

CameraRenderer::CameraRenderer(void):
    m_camera(nullptr),
    m_viewRendering(-1)
{
}

CameraRenderer::~CameraRenderer(void)
{
}

View &CameraRenderer::currentView(void)
{
    return m_views[!m_viewRendering];
}

void CameraRenderer::render(void)
{
    if (Q_UNLIKELY(m_viewRendering < 0))
        return;

    auto &view = m_views[m_viewRendering];

    m_glRenderer.beginFrame();

    // TODO render view...

    m_glRenderer.endFrame();

    QQuickOpenGLUtils::resetOpenGLState();
}

void CameraRenderer::synchronize(QQuickFramebufferObject *item)
{
    if (m_viewRendering >= 0) {
        m_viewRendering = !m_viewRendering;
        return;
    }

    if (!m_glRenderer.init(QSize(item->width(), item->height())))
        return;

    auto camera = qobject_cast<Camera *>(item);
    if (m_camera == camera)
        return;

    m_camera = camera;

    if (m_camera == nullptr)
        return;

    // TODO

    auto world = m_camera->world();
    if (world == nullptr)
        return;

    // TODO

    m_viewRendering = 1;
}

void CameraRenderer::addObject(Object *object)
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

void CameraRenderer::addEntity(Entity *entity)
{
    auto components = entity->components();
    for (auto component: components) {

    }
}

void CameraRenderer::addDirectionalLight(DirectionalLight *directionalLight)
{

}

void CameraRenderer::addPointLight(PointLight *pointLight)
{

}

void CameraRenderer::addSpotLight(SpotLight *spotLight)
{

}

VOLCANO_GRAPHICS_END
