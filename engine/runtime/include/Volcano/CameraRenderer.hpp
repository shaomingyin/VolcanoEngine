//
//
#ifndef VOLCANO_CAMERARENDERER_HPP
#define VOLCANO_CAMERARENDERER_HPP

#include <QQuickItem>
#include <QQuickFramebufferObject>

#include <Volcano/Common.hpp>
#include <Volcano/Entity.hpp>
#include <Volcano/Mesh.hpp>
#include <Volcano/DirectionalLight.hpp>
#include <Volcano/PointLight.hpp>
#include <Volcano/SpotLight.hpp>
#include <Volcano/World.hpp>
#include <Volcano/VisibleSet.hpp>
#include <Volcano/OpenGLMesh.hpp>
#include <Volcano/OpenGLMaterial.hpp>
#include <Volcano/OpenGLLight.hpp>
#include <Volcano/OpenGLDirectionalLight.hpp>
#include <Volcano/OpenGLPointLight.hpp>
#include <Volcano/OpenGLSpotLight.hpp>
#include <Volcano/OpenGLRenderer.hpp>

VOLCANO_BEGIN

class CameraView;

class CameraRenderer: public QQuickFramebufferObject::Renderer {
public:
    CameraRenderer(void);
    ~CameraRenderer(void) override;

public:
    void updateVisibleSet(void);

protected:
    void render(void) override;
    void renderVisibleSet(const VisibleSet &vs);
    void synchronize(QQuickFramebufferObject *item) override;
    void addObject(Object *object);
    void addEntity(Entity *entity);
    void addDirectionalLight(DirectionalLight *directionalLight);
    void addPointLight(PointLight *pointLight);
    void addSpotLight(SpotLight *spotLight);

private:
    CameraView *m_cameraView;
    const VisibleSet *m_visibleSet;
    OpenGLRenderer m_glRenderer;
};

VOLCANO_END

#endif // VOLCANO_CAMERARENDERER_HPP
