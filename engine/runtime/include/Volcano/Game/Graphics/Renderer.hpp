//
//
#ifndef VOLCANO_GAME_GRAPHICS_RARENDERER_HPP
#define VOLCANO_GAME_GRAPHICS_RARENDERER_HPP

#include <QQuickItem>
#include <QQuickFramebufferObject>

#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Mesh.hpp>
#include <Volcano/Game/DirectionalLight.hpp>
#include <Volcano/Game/PointLight.hpp>
#include <Volcano/Game/SpotLight.hpp>
#include <Volcano/Game/World.hpp>
#include <Volcano/Game/Graphics/View.hpp>

#include <Volcano/OpenGL/Mesh.hpp>
#include <Volcano/OpenGL/Material.hpp>
#include <Volcano/OpenGL/Renderer.hpp>

#include <Volcano/Game/Graphics/Common.hpp>

VOLCANO_GAME_GRAPHICS_BEGIN

class Camera;

class Renderer: public QQuickFramebufferObject::Renderer {
public:
    Renderer(void);
    ~Renderer(void) override;

public:
    void updateVisibleSet(void);

protected:
    void render(void) override;
    void renderView(const View &view);
    void synchronize(QQuickFramebufferObject *item) override;
    void addObject(Object *object);
    void addEntity(Entity *entity);
    void addDirectionalLight(DirectionalLight *directionalLight);
    void addPointLight(PointLight *pointLight);
    void addSpotLight(SpotLight *spotLight);

private:
    Camera *m_camera;
    const View *m_view;
    OpenGL::Renderer m_glRenderer;
};

VOLCANO_GAME_GRAPHICS_END

#endif // VOLCANO_GAME_GRAPHICS_RENDERER_HPP
