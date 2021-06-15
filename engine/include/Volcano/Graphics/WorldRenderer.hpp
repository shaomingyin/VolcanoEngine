//
//
#ifndef VOLCANO_GRAPHICS_WORLDRENDERER_HPP
#define VOLCANO_GRAPHICS_WORLDRENDERER_HPP

#include <QMatrix4x4>
#include <QQuickItem>
#include <QQuickFramebufferObject>

#include <Volcano/Entity.hpp>
#include <Volcano/Mesh.hpp>
#include <Volcano/DirectionalLight.hpp>
#include <Volcano/PointLight.hpp>
#include <Volcano/SpotLight.hpp>
#include <Volcano/World.hpp>

#include <Volcano/OpenGL/Renderer.hpp>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Camera.hpp>

VOLCANO_GRAPHICS_BEGIN

class WorldRenderer: public QQuickFramebufferObject::Renderer {
public:
    WorldRenderer(void);
    ~WorldRenderer(void) override;

protected:
    void render(void) override;
    void synchronize(QQuickFramebufferObject *item) override;
    void attachCamera(Camera *camera);
    void addObject(Object *object);
    void addEntity(Entity *entity);
    void addDirectionalLight(DirectionalLight *directionalLight);
    void addPointLight(PointLight *pointLight);
    void addSpotLight(SpotLight *spotLight);
    void buildVisibleSet(void);

private slots:

private:
    Camera *m_camera;
    OpenGL::Renderer *m_glRenderer;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_WORLDRENDERER_HPP
