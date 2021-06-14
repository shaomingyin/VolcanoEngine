//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_HPP
#define VOLCANO_GRAPHICS_RENDERER_HPP

#include <QQuickFramebufferObject>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Renderer: public QQuickFramebufferObject::Renderer {
public:
    Renderer(void);
    ~Renderer(void) override;

protected:
    void render(void) override;
    void synchronize(QQuickFramebufferObject *item) override;

private:
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_HPP
