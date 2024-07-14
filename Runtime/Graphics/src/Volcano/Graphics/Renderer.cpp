//
//
#include <Volcano/Graphics/Renderer.hpp>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(QObject* parent)
    : QObject(parent)
    , is_inited_(false) {
}

bool Renderer::init(int width, int height) {
    if (is_inited_) {
        return true;
    }

    if (!doInit(width, height)) {
        return false;
    }

    size_ = QSize(width, height);
    is_inited_ = true;

    return true;
}

bool Renderer::isInited() const {
    return is_inited_;
}

bool Renderer::doInit(int width, int height) {
    return false;
}

VOLCANO_GRAPHICS_END
