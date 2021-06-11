//
//
#ifndef VOLCANO_UI_WORLDRENDERER_HPP
#define VOLCANO_UI_WORLDRENDERER_HPP

#include <QQuickFramebufferObject>

#include <Volcano/UI/Common.hpp>

VOLCANO_UI_BEGIN

class WorldRenderer: public QQuickFramebufferObject::Renderer {
public:
    WorldRenderer(void);
    ~WorldRenderer(void) override;

protected:
    void render(void) override;
    void synchronize(QQuickFramebufferObject *item) override;
};

VOLCANO_UI_END

#endif // VOLCANO_UI_WORLDRENDERER_HPP
