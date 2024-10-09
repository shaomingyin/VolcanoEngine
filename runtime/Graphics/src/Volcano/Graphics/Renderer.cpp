//
//
#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer()
    : driver_(nullptr)
    , widht_(0)
    , height_(0) {
}

Renderer::~Renderer() {
}

bool Renderer::init(int width, int height) {
    widht_ = width;
    height_ = height;
    return true;
}

bool Renderer::setDriver(Driver* p) {
    driver_ = p;
    return true;
}

bool Renderer::begin() {
    return true;
}

void Renderer::end() {
}

void Renderer::update(std::chrono::steady_clock::duration elapsed) {
    VOLCANO_ASSERT(driver_ != nullptr);
}

VOLCANO_GRAPHICS_END
