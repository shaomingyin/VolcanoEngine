//
//
#ifndef VOLCANO_GAME_GRAPHICS_RARENDERER_HPP
#define VOLCANO_GAME_GRAPHICS_RARENDERER_HPP

#include <QList>
#include <QQuickFramebufferObject>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/VisibleSet.hpp>

VOLCANO_GRAPHICS_BEGIN

class Camera;

class Renderer: public QQuickFramebufferObject::Renderer {
public:
    Renderer(Camera &camera);
    ~Renderer(void) override;

public:
    bool init(void);

protected:
    void render(void) override;
    void synchronize(QQuickFramebufferObject *item) override;

private:
    Camera &m_camera;
    const VisibleSet *m_vs;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_HPP
