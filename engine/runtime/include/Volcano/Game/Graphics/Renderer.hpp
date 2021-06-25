//
//
#ifndef VOLCANO_GAME_GRAPHICS_RARENDERER_HPP
#define VOLCANO_GAME_GRAPHICS_RARENDERER_HPP

#include <QList>
#include <QQuickFramebufferObject>

#include <Volcano/OpenGL/Renderer.hpp>

#include <Volcano/Game/World.hpp>
#include <Volcano/Game/Entity.hpp>

#include <Volcano/Game/Graphics/Common.hpp>
#include <Volcano/Game/Graphics/Light.hpp>
#include <Volcano/Game/Graphics/DirectionalLight.hpp>
#include <Volcano/Game/Graphics/PointLight.hpp>
#include <Volcano/Game/Graphics/SpotLight.hpp>
#include <Volcano/Game/Graphics/Mesh.hpp>
#include <Volcano/Game/Graphics/Material.hpp>
#include <Volcano/Game/Graphics/View.hpp>

VOLCANO_GAME_GRAPHICS_BEGIN

class Camera;

class Renderer: public QQuickFramebufferObject::Renderer {
public:
    Renderer(Camera &camera);
    ~Renderer(void) override;

public:
    bool init(void);

protected:
    void reset(void);
    void render(void) override;
    void renderView(const View &view);
    void synchronize(QQuickFramebufferObject *item) override;
    void addObject(Object *object);
    void addEntity(Entity *entity);
    void addMesh(Mesh *mesh);
    void addDirectionalLight(DirectionalLight *directionalLight);
    void addPointLight(PointLight *pointLight);
    void addSpotLight(SpotLight *spotLight);

private slots:
    void attachWorld(World *world);

private:
    Camera &m_camera;
    World *m_world;
    const View *m_view;
    OpenGL::Renderer m_glRenderer;
};

VOLCANO_GAME_GRAPHICS_END

#endif // VOLCANO_GAME_GRAPHICS_RENDERER_HPP
