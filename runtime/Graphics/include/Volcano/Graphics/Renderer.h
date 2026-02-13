//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <GL/gl3w.h>

#include <SFML/Graphics/RenderTarget.hpp>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Context.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer {
public:
    Renderer(Context& contet, sf::RenderTarget& target);
    virtual ~Renderer() = default;

public:
    void render(const View& view);

private:
    Context& context_;
    sf::RenderTarget& target_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
