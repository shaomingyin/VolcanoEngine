//
//
#include <memory>

#include <QQuickOpenGLUtils>

#include <Volcano/CameraView.hpp>
#include <Volcano/CameraRenderer.hpp>

VOLCANO_BEGIN

CameraRenderer::CameraRenderer(void):
    m_cameraView(nullptr),
    m_visibleSet(nullptr)
{
}

CameraRenderer::~CameraRenderer(void)
{
}

void CameraRenderer::render(void)
{
    if (Q_UNLIKELY(m_cameraView == nullptr || m_visibleSet == nullptr))
        return;

    m_glRenderer.beginFrame();

    // TODO

    m_glRenderer.endFrame();

    m_cameraView->unlockVisibleSet();

    QQuickOpenGLUtils::resetOpenGLState();
}

void CameraRenderer::synchronize(QQuickFramebufferObject *item)
{
    if (Q_UNLIKELY(m_cameraView == nullptr)) {
        auto cameraView = qobject_cast<CameraView *>(item);
        if (cameraView == nullptr)
            return;

        auto world = cameraView->world();
        if (world == nullptr)
            return;

        if (!m_glRenderer.init(item->width(), item->height()))
            return;

        auto &objects = world->objects();
        for (auto object: objects)
            addObject(object);

        m_cameraView = cameraView;
    }

    m_glRenderer.resize(QSize(item->width(), item->height()));
    m_visibleSet = m_cameraView->lockVisibleSet();
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
        auto mesh = qobject_cast<Mesh *>(component);
        if (mesh != nullptr) {
        }
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

VOLCANO_END
