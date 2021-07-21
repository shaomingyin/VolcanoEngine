//
//
#ifndef VOLCANO_GAME_GRAPHICS_RARENDERER_HPP
#define VOLCANO_GAME_GRAPHICS_RARENDERER_HPP

#include <QQuickFramebufferObject>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/View.hpp>

VOLCANO_GRAPHICS_BEGIN

class Renderer: public QQuickFramebufferObject::Renderer {
public:
    Renderer(void);
    ~Renderer(void) override;

public:
    bool init(void);
    void render(void) override;
    void synchronize(QQuickFramebufferObject *item) override;

private:
    View *m_view;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_HPP
