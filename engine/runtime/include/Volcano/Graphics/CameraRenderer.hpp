//
//
#ifndef VOLCANO_GRAPHICS_CAMERARENDERER_HPP
#define VOLCANO_GRAPHICS_CAMERARENDERER_HPP

#include <QList>
#include <QPair>
#include <QMatrix4x4>
#include <QQuickItem>
#include <QQuickFramebufferObject>

#include <Volcano/Entity.hpp>
#include <Volcano/Mesh.hpp>
#include <Volcano/DirectionalLight.hpp>
#include <Volcano/PointLight.hpp>
#include <Volcano/SpotLight.hpp>
#include <Volcano/World.hpp>

#include <Volcano/OpenGL/Mesh.hpp>
#include <Volcano/OpenGL/Material.hpp>
#include <Volcano/OpenGL/Light.hpp>
#include <Volcano/OpenGL/DirectionalLight.hpp>
#include <Volcano/OpenGL/PointLight.hpp>
#include <Volcano/OpenGL/SpotLight.hpp>
#include <Volcano/OpenGL/Renderer.hpp>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/View.hpp>

VOLCANO_GRAPHICS_BEGIN

class Camera;

class CameraRenderer: public QQuickFramebufferObject::Renderer {
public:
    CameraRenderer(void);
    ~CameraRenderer(void) override;

public:
    View &currentView(void);

protected:
    void render(void) override;
    void synchronize(QQuickFramebufferObject *item) override;
    void addObject(Object *object);
    void addEntity(Entity *entity);
    void addDirectionalLight(DirectionalLight *directionalLight);
    void addPointLight(PointLight *pointLight);
    void addSpotLight(SpotLight *spotLight);

private:
    Camera *m_camera;
    View m_views[2];
    int m_viewRendering;
    OpenGL::Renderer m_glRenderer;
    OpenGL::Memory m_glMemory;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CAMERARENDERER_HPP
