#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(World::Scene& scene)
    : scene_(scene) {
    spdlog::info("Initializing graphics renderer...");
}

Renderer::~Renderer() {
}

void Renderer::build() noexcept {
}

void Renderer::resize(int width, int height) noexcept {
    width_ = width;
    height_ = height;
    frame_.setViewport(width, height);
}

void Renderer::beginFrame() noexcept {
    frame_.reset();
    frame_.setViewport(width_, height_);
}

void Renderer::render() {
    frame_.commit(0, 0, width_, height_);
}

VOLCANO_GRAPHICS_END
