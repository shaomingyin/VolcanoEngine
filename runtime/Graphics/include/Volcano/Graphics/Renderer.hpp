//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_HPP
#define VOLCANO_GRAPHICS_RENDERER_HPP

#include <GL/gl3w.h>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Renderer {
public:
    Renderer(int width, int height);
    virtual ~Renderer() = default;

public:
    int width() const noexcept {
        return width_;
    }

    int height() const noexcept {
        return height_;
    }

    void resize(int width, int height);
    void beginFrame();
    void endFrame();

private:
    int width_;
    int height_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_HPP
