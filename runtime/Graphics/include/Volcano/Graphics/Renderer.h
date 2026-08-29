//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Frame.h>
#include <Volcano/World/Scene.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer {
public:
    Renderer(World::Scene& scene);
    virtual ~Renderer();

public:
    virtual void build() noexcept;
    virtual void resize(int width, int height) noexcept;
    virtual void render();

    void beginFrame() noexcept;

    Frame& frame() noexcept {
        return frame_;
    }

    const Frame& frame() const noexcept {
        return frame_;
    }

    int width() const noexcept {
        return width_;
    }

    int height() const noexcept {
        return height_;
    }

protected:
    World::Scene& scene() noexcept {
        return scene_;
    }

    const World::Scene& scene() const noexcept {
        return scene_;
    }

private:
    World::Scene& scene_;
    Frame frame_;
    int width_ = 1;
    int height_ = 1;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
