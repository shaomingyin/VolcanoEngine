//
//
#ifndef VOLCANO_GAME_GRAPHICS_RARENDERER_HPP
#define VOLCANO_GAME_GRAPHICS_RARENDERER_HPP

#include <QList>
#include <QQuickFramebufferObject>

#include <Volcano/Game/World.hpp>
#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Light.hpp>
#include <Volcano/Game/DirectionalLight.hpp>
#include <Volcano/Game/PointLight.hpp>
#include <Volcano/Game/SpotLight.hpp>
#include <Volcano/Game/Mesh.hpp>
#include <Volcano/Game/Material.hpp>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/View.hpp>

VOLCANO_GRAPHICS_BEGIN

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
    void addObject(Game::Object *object);
    void addEntity(Game::Entity *entity);
    void addMesh(Game::Mesh *mesh);
    void addDirectionalLight(Game::DirectionalLight *directionalLight);
    void addPointLight(Game::PointLight *pointLight);
    void addSpotLight(Game::SpotLight *spotLight);

private slots:
    void attachGameWorld(Game::World *gameWorld);

private:
    Camera &m_camera;
    Game::World *m_gameWorld;
    const View *m_view;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_HPP
