//
//
#include <Volcano/Graphics/Viewport.h>

VOLCANO_GRAPHICS_BEGIN

class ViewportRenderer: public QQuickFramebufferObject::Renderer {
public:

public:
    void render() override {

    }

    void synchronize(QQuickFramebufferObject* fb) override {

    }

private:
};

Viewport::Viewport(QQuickItem* parent)
    : QQuickFramebufferObject(parent)
    , camera_(nullptr) {
}

Viewport::Renderer *Viewport::createRenderer() const {
    return new ViewportRenderer();
}

VOLCANO_GRAPHICS_END
