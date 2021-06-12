//
//
#ifndef VOLCANO_CLIENT_WORLDRENDERER_HPP
#define VOLCANO_CLIENT_WORLDRENDERER_HPP

#include <QQuickFramebufferObject>

#include <Volcano/OpenGL/Renderer.hpp>
#include <Volcano/Client/Common.hpp>

VOLCANO_CLIENT_BEGIN

class WorldRenderer: public QQuickFramebufferObject::Renderer {
public:
    WorldRenderer(void);
    ~WorldRenderer(void) override;

protected:
    void render(void) override;
    void synchronize(QQuickFramebufferObject *item) override;

private:
    OpenGL::Renderer *m_glRenderer;
};

VOLCANO_CLIENT_END

#endif // VOLCANO_CLIENT_WORLDRENDERER_HPP
